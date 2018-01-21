/*
 * lootWindow.cpp
 *
 *  Created on: 29 Sep 2016
 *      Author: linbob
 */

#include "lootWindow.hpp"
#include "../../gameScreen.hpp"

/*
 * - scrollable list area
 * 		- right click context menu
 * 			- view item information
 * 			- use item(ring bell, use key, read note/book)
 * 			- equip wearable item
 * 			- consume item
 * - take selected button action
 */

void LootWindow::init(GameScreen* gs){
	this->theme = gs->getManager()->getTheme();
	this->overWorld = gs->getOverworld();
	this->factory = &gs->getEntFactory();

	connect("closed", [this]{
		this->hide();
	});

	auto itemArea = std::make_shared<ItemArea>(gs);
	itemArea->setPosition(0, 0);
	itemArea->setSize(getSize().x, getSize().y * 0.9);
	add(itemArea, "itemArea");

	itemArea->getContext()->createButton("Take", "take", [itemArea, this]{
		ItemContext::Ptr context = itemArea->getContext();
		context->hide();

		anax::Entity player = factory->getPlayer();
		InventoryComponent& inven = player.getComponent<InventoryComponent>();

		//move item to player inventory
		if(!inven.addItem(context->getSelected()->getEntity())){
			printf("Your bag is too heavy. You cannot carry anything else");
			return;
		}

		//remove item from loot drop
		anax::Entity container = factory->world->getEntity(itemArea->getParentEntity());
		InventoryComponent& cInven = container.getComponent<InventoryComponent>();
		cInven.removeItem(context->getSelected()->getEntity().getId().index);

		if(cInven.inventory.empty()){
			//loot drop is empty - remove it from world
			this->hide();
			PositionComponent& p = container.getComponent<PositionComponent>();
			this->overWorld->child->zone->getEntityLayer().removeEntity(
					p.tilePosition.x, p.tilePosition.y, container.getId().index);
			container.kill();
		}else{
			itemArea->updateContents(container);
		}
	});

	//take selected button
	tgui::Button::Ptr takeSelected = theme->load("Button");
	takeSelected->setSize(108, 30);
	takeSelected->setTextSize(14);
	takeSelected->setText("Take all");
	add(takeSelected);
	takeSelected->setPosition(itemArea->getSize().x * 0.5 - takeSelected->getSize().x * 1.1,
			itemArea->getSize().y);

	hide();
}

void LootWindow::show(float x, float y){
	this->x = x;
	this->y = y;

	for(EntityRefNode& ref : overWorld->child->zone->getEntityLayer().getEntitiesAt(x, y)){
		anax::Entity e = factory->world->getEntity(ref.id);
		std::string refName = e.getComponent<NameComponent>().name;
		if(refName.compare("Loot drop") == 0){
			get<ItemArea>("itemArea")->updateContents(e);
		}
	}

	tgui::ChildWindow::show();
}
void LootWindow::updateContents(int tgtX, int tgtY){
	this->x = tgtX;
	this->y = tgtY;
	get<ItemArea>("itemArea")->updateContents(factory->getPlayer());
}
bool LootWindow::rightClick(float x, float y){
	if(!isVisible()) return false;

	//check if right click is in item area
	ItemArea::Ptr itemArea = get<ItemArea>("itemArea");
	bool insideArea = sf::FloatRect{itemArea->getAbsolutePosition().x, itemArea->getAbsolutePosition().y,
		itemArea->getSize().x, itemArea->getSize().y}.contains(x, y);
	if(insideArea){
		for(tgui::Widget::Ptr w : get<tgui::Panel>("entryArea", true)->getWidgets()){
			ItemEntry::Ptr entry = std::dynamic_pointer_cast<ItemEntry>(w);
			if(entry){
				bool entryArea = sf::FloatRect{entry->getAbsolutePosition().x, entry->getAbsolutePosition().y,
					entry->getSize().x, entry->getSize().y}.contains(x, y);
				if(entryArea){
					ItemContext::Ptr context = itemArea->getContext();
					context->moveToFront();

					int count = 0;

					context->show(x, y, entry);

					context->addButton("take", count++);

					return true;
				}
			}
		}
	}

	return false;
}
