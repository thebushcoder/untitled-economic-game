/*
 * shopWindow.cpp
 *
 *  Created on: 16 Oct 2016
 *      Author: linbob
 */
#include "shopWindow.hpp"
#include "../../gameScreen.hpp"

void ShopWindow::init(GameScreen* gs){
	this->lootManager = gs->getLootManager();
	ItemExchangeWindow::init(gs);

	ItemArea::Ptr rightArea = get<ItemArea>("right_area");
	ItemArea::Ptr leftArea = get<ItemArea>("left_area");

	rightArea->getContext()->createButton("Sell", "sell", [rightArea]{

		rightArea->getContext()->hide();
	});

	leftArea->getContext()->createButton("Buy", "buy", [leftArea]{

		leftArea->getContext()->hide();
	});

	//SELL TO SHOP
	get<tgui::Button>("R_to_L")->connect("pressed", [rightArea, leftArea]{
		if(!rightArea->isMarkedEmpty()){
			float v = 0;
			for(auto e : rightArea->getMarked()){
				ValueComponent& val = e->getEntity().getComponent<ValueComponent>();

				v += val.getValue();
			}

			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << v;

			std::string msg = "Confirm: Sell " + std::to_string(rightArea->getMarked().size()) +
					" items for $" + stream.str();

			ConfirmDialog::Ptr confWin = rightArea->getContext()->getConfDialog();
			confWin->get<tgui::Label>("label")->setTextSize(14);
			confWin->moveToFront();
			confWin->show(msg, [confWin]{
				confWin->hide();
			});
		}
	});

	//BUY FROM SHOP


}
void ShopWindow::show(anax::Entity shopEntity){
	loadShopItems(shopEntity);
	get<ItemArea>("right_area")->updateContents(factory->getPlayer());
	tgui::ChildWindow::show();
}
void ShopWindow::loadShopItems(anax::Entity shop){
	//generate stock
	InventoryComponent& inven = shop.getComponent<InventoryComponent>();
	if(inven.inventory.size() < 3){
		LootListComponent& shopLoot = shop.getComponent<LootListComponent>();
		std::vector<anax::Entity> loot = lootManager->generateLoot(shopLoot.list);
		for(anax::Entity& item : loot){
			inven.inventory.push_back(item.getId().index);
		}
	}

	get<ItemArea>("left_area")->updateContents(shop);
}
bool ShopWindow::rightClick(float x, float y){
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

////////////////////////////////////////////////////

