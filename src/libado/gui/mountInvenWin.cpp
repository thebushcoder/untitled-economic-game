/*
 * mountInvenWin.cpp
 *
 *  Created on: 23 Sep 2017
 *      Author: linbob
 */

#include "mountInvenWin.hpp"
#include "../../gameScreen.hpp"

void MountInvenWindow::init(GameScreen* gs){
	ItemExchangeWindow::init(gs);
	this->world = gs->getWorld();

	ItemArea::Ptr rightArea = get<ItemArea>("right_area");
	ItemArea::Ptr leftArea = get<ItemArea>("left_area");

	rightArea->getContext()->createButton("Store", "store", [this, rightArea, leftArea]{
		anax::Entity player = this->factory->getPlayer();
		InventoryComponent& playerInven = player.getComponent<InventoryComponent>();
		anax::Entity mount = this->world->getEntity(leftArea->getParentEntity());
		InventoryComponent& mountInven = mount.getComponent<InventoryComponent>();

		anax::Entity selected = rightArea->getContext()->getSelected()->getEntity();

		mountInven.addItem(selected);
		playerInven.removeItem(selected.getId().index);

		leftArea->updateContents(mount);
		rightArea->updateContents(player);

		rightArea->getContext()->hide();
	});

	leftArea->getContext()->createButton("Take", "take", [this, rightArea, leftArea]{
		anax::Entity player = this->factory->getPlayer();
		InventoryComponent& playerInven = player.getComponent<InventoryComponent>();
		anax::Entity mount = this->world->getEntity(leftArea->getParentEntity());
		InventoryComponent& mountInven = mount.getComponent<InventoryComponent>();

		anax::Entity selected = leftArea->getContext()->getSelected()->getEntity();

		playerInven.addItem(selected);
		mountInven.removeItem(selected.getId().index);

		leftArea->updateContents(mount);
		rightArea->updateContents(player);

		leftArea->getContext()->hide();
	});

	//store in mount
	get<tgui::Button>("R_to_L")->connect("pressed", [rightArea, leftArea]{
		if(!rightArea->isMarkedEmpty()){

			std::string msg = "Confirm: Store " + std::to_string(rightArea->getMarked().size()) +
					" items";

			ConfirmDialog::Ptr confWin = rightArea->getContext()->getConfDialog();
			confWin->get<tgui::Label>("label")->setTextSize(14);
			confWin->moveToFront();
			confWin->show(msg, [confWin]{

				confWin->hide();
			});
		}
	});

	//take FROM mount
	get<tgui::Button>("R_to_L")->connect("pressed", [rightArea, leftArea]{
		if(!rightArea->isMarkedEmpty()){

			std::string msg = "Confirm: Take " + std::to_string(rightArea->getMarked().size()) +
					" items";

			ConfirmDialog::Ptr confWin = rightArea->getContext()->getConfDialog();
			confWin->get<tgui::Label>("label")->setTextSize(14);
			confWin->moveToFront();
			confWin->show(msg, [confWin]{

				confWin->hide();
			});
		}
	});

}
void MountInvenWindow::show(anax::Entity mount){
	get<ItemArea>("left_area")->updateContents(mount);
	get<ItemArea>("right_area")->updateContents(factory->getPlayer());
	tgui::ChildWindow::show();
}
bool MountInvenWindow::rightClick(float x, float y){
	if(!isVisible()) return false;

	//check if right click is in item area
	ItemArea::Ptr items = get<ItemArea>("left_area", true);
	bool itemArea = sf::FloatRect{items->getAbsolutePosition().x, items->getAbsolutePosition().y,
		items->getSize().x, items->getSize().y}.contains(x, y);
	if(itemArea){
		for(tgui::Widget::Ptr w : items->getEntryArea()->getWidgets()){
			ItemEntry::Ptr entry = std::dynamic_pointer_cast<ItemEntry>(w);
			if(entry){
				bool entryArea = sf::FloatRect{entry->getAbsolutePosition().x, entry->getAbsolutePosition().y,
					entry->getSize().x, entry->getSize().y}.contains(x, y);
				if(entryArea){
					ItemContext::Ptr context = items->getContext();
					context->moveToFront();
					context->show(x, y, entry);

					int count = 0;

					context->addButton("take", count++);

					if(!items->isMarked(entry)){
						context->addButton("mark", count++);
					}

					if(!items->isMarkedEmpty()){
						if(items->isMarked(entry)){
							context->addButton("remove_mark", count++);
						}
						context->addButton("removeAll_marks", count++);
					}

					return true;
				}
			}
		}
	}

	items = get<ItemArea>("right_area", true);
	itemArea = sf::FloatRect{items->getAbsolutePosition().x, items->getAbsolutePosition().y,
		items->getSize().x, items->getSize().y}.contains(x, y);
	if(itemArea){
		for(tgui::Widget::Ptr w : items->getEntryArea()->getWidgets()){
			ItemEntry::Ptr entry = std::dynamic_pointer_cast<ItemEntry>(w);
			if(entry){
				bool entryArea = sf::FloatRect{entry->getAbsolutePosition().x, entry->getAbsolutePosition().y,
					entry->getSize().x, entry->getSize().y}.contains(x, y);
				if(entryArea){
					ItemContext::Ptr context = items->getContext();
					context->moveToFront();
					context->show(x, y, entry);

					int count = 0;

					context->addButton("store", count++);

					if(!items->isMarked(entry)){
						context->addButton("mark", count++);
					}

					if(!items->isMarkedEmpty()){
						if(items->isMarked(entry)){
							context->addButton("remove_mark", count++);
						}
						context->addButton("removeAll_marks", count++);
					}

					return true;
				}
			}
		}
	}
	return false;
}


