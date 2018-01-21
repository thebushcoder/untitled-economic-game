/*
 * inventoryWindow.cpp
 *
 *  Created on: 7 Nov 2016
 *      Author: linbob
 */

#include "inventoryWindow.hpp"
#include "../../gameScreen.hpp"

void InventoryWindow::equipItem(ItemEntry::Ptr i){
	InventoryWindow::Ptr invenWin = getParent()->get<InventoryWindow>("inventory_window");

	//find slot widget for item type
	ItemComponent& itemType = i->getEntity().getComponent<ItemComponent>();
	for(Item::Type t : itemType.types){
		for(tgui::Widget::Ptr w : invenWin->getWidgets()){
			if(SlotWidget::Ptr slotW = std::dynamic_pointer_cast<SlotWidget>(w)){
				if(slotW->getType() == t){
					EquipmentSlot* es = (gameScreen->getEntFactory().getPlayer().getComponent<EquipmentComponent>()).getSlot(slotW->getType());

					//try to equip item
					if(!es->equipItem(i->getEntity())){
						//unable to equip item
						return;
					}

					//update widget with item icon + name
					slotW->getEntry()->updateEntry(i->getEntity());
					invenWin->attachTooltip(slotW->getEntry());

					//remove item from player's inven
					InventoryComponent& inven = gameScreen->getEntFactory().getPlayer().getComponent<InventoryComponent>();
					inven.removeItem(i->getEntity().getId().index);

					//remove inven entry from inven window
					tgui::Panel::Ptr entryContainer = invenWin->get<tgui::Panel>("entryArea", true);
					entryContainer->remove(i);
					invenWin->updateEntries();

					return;
				}
			}
		}
	}
}
void InventoryWindow::init(GameScreen* gs){
	this->theme = gs->getManager()->getTheme();
	this->gameScreen = gs;

	connect("closed", [this]{
		this->hide();
	});

	itemTooltip = std::make_shared<ItemTooltip>(theme);

	int buttonH = 28;

	tgui::Button::Ptr button = theme->load("Button");
	button->setText("Stats");
	button->setSize(52, buttonH);
	button->setPosition(4, 0);
	add(button);

	button = theme->load("Button");
	button->setText("Inventory");
	button->setSize(62, buttonH);
	button->setPosition(52 + 8, 0);
	add(button);

	int slotW = getSize().x * 0.29, slotH = getSize().y * 0.10;

	//EQUIPMENT SLOTS
	SlotWidget::Ptr outfitSlot = std::make_shared<SlotWidget>("Outfit:", Item::OUTFIT, theme);
	outfitSlot->setSize(slotW, slotH);
	outfitSlot->setPosition(4 , buttonH + 8);
	outfitSlot->init();
	add(outfitSlot, "outfit_slot");
	equipmentSlots.push_back(outfitSlot);

	SlotWidget::Ptr mainHandSlot = std::make_shared<SlotWidget>("Main-hand:", Item::MAIN_HAND, theme);
	mainHandSlot->setSize(slotW, slotH);
	mainHandSlot->setPosition(4 , outfitSlot->getPosition().y + slotH + 4);
	mainHandSlot->init();
	add(mainHandSlot, "mainHand_slot");
	equipmentSlots.push_back(mainHandSlot);

	SlotWidget::Ptr offHandSlot = std::make_shared<SlotWidget>("Off-hand:", Item::OFF_HAND, theme);
	offHandSlot->setSize(slotW, slotH);
	offHandSlot->setPosition(4 , mainHandSlot->getPosition().y + slotH + 4);
	offHandSlot->init();
	add(offHandSlot, "offHand_slot");
	equipmentSlots.push_back(offHandSlot);

	SlotWidget::Ptr mountSlot = std::make_shared<SlotWidget>("Mount:", Item::MOUNT, theme);
	mountSlot->setSize(slotW, slotH);
	mountSlot->setPosition(4 , offHandSlot->getPosition().y + slotH + 4);
	mountSlot->init();
	add(mountSlot, "mount_slot");
	equipmentSlots.push_back(mountSlot);

	for(int i = 1; i <= MAX_TRINKETS; ++i){
		std::string n = "trinket_" + i;
		n += "_slot";

		SlotWidget::Ptr trinketSlot = std::make_shared<SlotWidget>("Trinket:", Item::TRINKET, theme);
		trinketSlot->setSize(slotW, slotH);
		trinketSlot->setPosition(4, mountSlot->getPosition().y + (slotH + 4) * i);
		trinketSlot->init();
		add(trinketSlot, n);
		equipmentSlots.push_back(trinketSlot);
	}

	////////////////////
	//INVENTORY WEIGHT
	int labelW = 60;

	tgui::Label::Ptr label = theme->load("Label");
	label->setText("Carried weight:");
	label->setTextSize(14);
	label->setPosition(4, getSize().y - buttonH);
	label->setSize(labelW * 2.5, buttonH);
	add(label);

	tgui::Label::Ptr weightLabel = theme->load("Label");
	weightLabel->setText("");
	weightLabel->setTextSize(12);
	weightLabel->setPosition(label->getSize().x * 0.85,	getSize().y - buttonH);
	weightLabel->setSize(labelW, buttonH);
	add(weightLabel, "weight_label");

	/////////////////

	//INVENTORY ITEMS AREA
	auto itemArea = std::make_shared<ItemArea>(gs);
	itemArea->setPosition(getSize().x * 0.36, button->getSize().y + 8);
	itemArea->setSize(getSize().x * 0.64, getSize().y * 0.9);
	add(itemArea, "itemArea");

	itemArea->getContext()->createButton("Equip", "equip", [&]{
		equipItem(itemArea->getContext()->getSelected());
		this->hide();
	});

	itemArea->getContext()->createButton("Drop", "drop", [this, itemArea]{
		itemArea->getContext()->hide();

		if(itemArea->isMarkedEmpty()){
			ItemEntry::Ptr i = itemArea->getContext()->getSelected();
			dropItem(i);

			this->updateEntries();
			anax::Entity player = this->gameScreen->getEntFactory().getPlayer();
			itemArea->updateContents(player);
		}else{
			ConfirmDialog::Ptr dialog = itemArea->getContext()->getConfDialog();

			std::string msg = "Confirm: Drop " +
					std::to_string(itemArea->getMarked().size()) + " items?";

			dialog->show(msg, [itemArea, dialog, this]{
				for(auto i : itemArea->getMarked()){
					this->dropItem(i);
				}

				this->updateEntries();
				anax::Entity player = this->gameScreen->getEntFactory().getPlayer();
				itemArea->updateContents(player);
				dialog->hide();
			});
		}
	});
	/////////////////////////

	int filterSize = 32;
	tgui::Button::Ptr filterB = theme->load("Button");
	filterB->setText("All");
	filterB->setSize(filterSize, filterSize);
	filterB->setPosition(itemArea->getPosition().x - (filterSize + 4),
			itemArea->getPosition().y);
	add(filterB, "filter_all");

	filterB = theme->load("Button");
	filterB->setText("Eq");
	filterB->setSize(filterSize, filterSize);
	filterB->setPosition(itemArea->getPosition().x - (filterSize + 4),
			itemArea->getPosition().y + filterSize);
	add(filterB, "filter_equip");

	filterB = theme->load("Button");
	filterB->setText("Com");
	filterB->setSize(filterSize, filterSize);
	filterB->setPosition(itemArea->getPosition().x - (filterSize + 4),
			itemArea->getPosition().y + filterSize * 2);
	add(filterB, "filter_commodity");

	hide();
}
void InventoryWindow::dropItem(ItemEntry::Ptr i){
	ItemArea::Ptr area = this->get<ItemArea>("itemArea");

	anax::Entity player = this->gameScreen->getEntFactory().getPlayer();

	PositionComponent& pos = player.getComponent<PositionComponent>();
	this->gameScreen->getOverworld()->getActiveZone()->zone->getEntityLayer().setEntity(
			pos.tilePosition.x, pos.tilePosition.y, i->getEntity());

	PositionComponent& itemPos = i->getEntity().getComponent<PositionComponent>();
	itemPos.setPosition(pos.tilePosition.x, pos.tilePosition.y,
			pos.screenPosition.x, pos.screenPosition.y);

	InventoryComponent& inven = player.getComponent<InventoryComponent>();
	inven.removeItem(i->getEntity().getId().index);
}
void InventoryWindow::show(){
	updateEntries();
	get<ItemArea>("itemArea")->clearMarked();
	get<ItemArea>("itemArea")->updateContents(gameScreen->getEntFactory().getPlayer());
	tgui::ChildWindow::show();
}
//load players inventory as item entries and gets inven weight
void InventoryWindow::updateEntries(){
	anax::Entity player = gameScreen->getEntFactory().getPlayer();
	InventoryComponent& inven = player.getComponent<InventoryComponent>();

	tgui::Label::Ptr weightLabel = get<tgui::Label>("weight_label", true);
	std::stringstream ss;
	ss << inven.count << " / " << inven.max;
	weightLabel->setText(ss.str());

	tgui::Panel::Ptr entryContainer = get<tgui::Panel>("entryArea", true);
	entryContainer->removeAllWidgets();

	EquipmentComponent& equip = player.getComponent<EquipmentComponent>();

	for(auto& s : equipmentSlots){
		EquipmentSlot* slot = equip.getSlot(s->getType());
		if(!slot->hasItem()) continue;

		s->getEntry()->updateEntry(gameScreen->getWorld()->getEntity(slot->getSlotItem()));
		attachTooltip(s->getEntry());
	}
}
bool InventoryWindow::rightClick(float x, float y){
	if(!isVisible()) return false;

	//check if right click is in item area
	ItemArea::Ptr items = get<ItemArea>("itemArea", true);
	bool itemArea = sf::FloatRect{items->getAbsolutePosition().x, items->getAbsolutePosition().y,
		items->getSize().x, items->getSize().y}.contains(x, y);
	if(itemArea){
		for(tgui::Widget::Ptr w : get<tgui::Panel>("entryArea", true)->getWidgets()){
			ItemEntry::Ptr entry = std::dynamic_pointer_cast<ItemEntry>(w);
			if(entry){
				bool entryArea = sf::FloatRect{entry->getAbsolutePosition().x, entry->getAbsolutePosition().y,
					entry->getSize().x, entry->getSize().y}.contains(x, y);
				if(entryArea){
					ItemContext::Ptr context = items->getContext();
					context->moveToFront();
					context->show(x, y, entry);

					int count = 0;

					ItemComponent& iC = entry->getEntity().getComponent<ItemComponent>();
					if(iC.hasType(Item::WEARABLE)){
						context->addButton("equip", count++);
					}

					if(!items->isMarked(entry)){
						context->addButton("mark", count++);
					}

					if(!items->isMarkedEmpty()){
						if(items->isMarked(entry)){
							context->addButton("remove_mark", count++);
						}
						context->addButton("removeAll_marks", count++);
					}

					if(gameScreen->getOverworld()->isChildActive()){
						context->addButton("drop", count++);
					}

					int c = context->getWidgets().size();
					context->setSize(context->getSize().x, (ItemContext::buttonHeight * c) + (4 * c));
					return true;
				}
			}
		}
	}else{
//		for(SlotWidget::Ptr& w : equipmentSlots){
//			if(sf::FloatRect{w->getEntry()->getAbsolutePosition().x,
//				w->getEntry()->getAbsolutePosition().y,
//				items->getSize().x, items->getSize().y}.contains(x, y)){
//				context->moveToFront();
//				context->show(x, y, true, w);
//				return true;
//			}
//		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////
