/*
 * contextMenu.cpp
 *
 *  Created on: 6 Sep 2017
 *      Author: linbob
 */

#include "itemArea.hpp"
#include "../../gameScreen.hpp"

void ConfirmDialog::init(tgui::Theme::Ptr theme){
	connect("closed", [this]{
		this->hide();
	});

	setTitle("Confirm action");

	tgui::Label::Ptr label = theme->load("Label");
	label->setSize(getSize().x * 0.95, 35);
	label->setPosition(8, 18);
	add(label, "label");

	int bHeight = 32;

	tgui::Button::Ptr button = theme->load("Button");
	button->setText("Confirm");
	button->setSize(getSize().x * 0.25, bHeight);
	button->setPosition(getSize().x * 0.05, getSize().y - bHeight - 8);
//	button->connect("pressed", func);
	add(button, "conf_button");

	button = theme->load("Button");
	button->setText("Cancel");
	button->setSize(getSize().x * 0.25, bHeight);
	button->setPosition(getSize().x - button->getSize().x - 8, getSize().y - bHeight - 8);
	button->connect("pressed", [this]{
		this->hide();
	});
	add(button);

	tgui::ChildWindow::hide();
}

ItemContext::ItemContext(GameScreen* gs, ItemArea* p) : tgui::Panel(), gameScreen(gs),
	theme(gs->getManager()->getTheme()), parent(p){

	setSize(180, buttonHeight * 5);
	setBackgroundColor(sf::Color(200, 200, 200));

	ConfirmDialog::Ptr d = theme->load("ConfirmDialog");
	d->setSize(242, 108);
	d->init(theme);
	dialog = d;

	//////////////
	createButton("Mark", "mark", [this]{
		this->parent->addMarked(this->getSelected());

		this->hide();
		return;
	});

	createButton("Remove mark", "remove_mark", [this]{
		this->parent->removeMarked(this->getSelected());
		this->hide();
		return;
	});

	createButton("Remove all marks", "removeAll_marks", [this]{
		this->parent->clearMarked();
		this->hide();
		return;
	});
	//////////////

	hide();
}
void ItemContext::show(int x, int y, ItemEntry::Ptr w){
	if(!dialog->getParent()){
		this->getParent()->add(dialog, "conf_window");
	}
	if(!getWidgets().empty()){
		removeAllWidgets();
	}

	selected = w;
	setPosition(x, y);

	int count = 1;

//	buttons["mark"]->setPosition(0, buttonHeight * count++ + 4);
//	add(buttons["mark"]);

	tgui::Panel::show();
}

void ItemContext::update(sf::Time elapsedTime){
	if(isVisible()){
		sf::Vector2i mousePos = sf::Mouse::getPosition(*gameScreen->getManager()->getWindow());
		sf::FloatRect area{getPosition().x - (getSize().x * 0.5), getPosition().y - (getSize().y * 0.4),
			getSize().x * 1.5, getSize().y * 1.4};

		if(!area.contains(mousePos.x, mousePos.y)){
			hide();
		}
	}
}

////////////////////////////////////////////

void ItemArea::scrollPanel(tgui::Panel::Ptr panel, tgui::Scrollbar::Ptr scroll)
{
	static int previousScrolbarValue;
    int distanceToMove = previousScrolbarValue - scroll->getValue();

    // Move all widgets that are inside the panel
    for (auto& widget : panel->getWidgets())
        widget->setPosition(widget->getPosition().x, widget->getPosition().y + distanceToMove);

    previousScrolbarValue = scroll->getValue();
}
ItemArea::ItemArea(GameScreen* gs) : tgui::HorizontalLayout(),
		gameScreen(gs){
//	setSize(getSize().x, getSize().y * 0.9);

	//item entry area
	auto entryContainer = std::make_shared<tgui::Panel>();
	entryContainer->setBackgroundColor(sf::Color::White);
	entryContainer->setSize(getSize().x, getSize().y);
	add(entryContainer, "entryArea");
	setRatio(entryContainer, 1.0f);

	context = std::make_shared<ItemContext>(gs, this);
	gs->getManager()->getGui()->add(context);

	//scrollbar
	tgui::Scrollbar::Ptr scrollbar = gameScreen->getManager()->getTheme()->load("Scrollbar");
	scrollbar->setSize(12, getSize().y);
	scrollbar->setArrowScrollAmount(60);
	scrollbar->setLowValue(getSize().y); // Viewable area (height of the panel)
	scrollbar->setMaximum(12 * entryHeight);
	add(scrollbar, "scroll");
	setRatio(scrollbar, 0.04f);
	scrollbar->connect("ValueChanged", scrollPanel, entryContainer, scrollbar);
}
void ItemArea::addMarked(ItemEntry::Ptr entry){
	marked.push_back(entry);
	entry->setBackgroundColor(sf::Color::Red);
}
ItemContext::Ptr ItemArea::getContext(){
	return context;
}
void ItemArea::show(){
	marked.clear();

	tgui::HorizontalLayout::show();
}
void ItemArea::hide(){
	marked.clear();

	tgui::HorizontalLayout::hide();
}
void ItemArea::updateContents(anax::Entity e){
	parentEntity = e.getId().index;

	InventoryComponent& inven = e.getComponent<InventoryComponent>();

	auto entryContainer = std::dynamic_pointer_cast<tgui::Panel>(get(0));
	entryContainer->removeAllWidgets();

	int count = 0;

	anax::World* w = gameScreen->getWorld();
	tgui::Theme::Ptr theme = gameScreen->getManager()->getTheme();

	for(int id: inven.inventory){
		anax::Entity i = w->getEntity(id);
		ItemEntry::Ptr entry = theme->load("ItemEntry");
		entry->setSize(entryContainer->getSize().x, entryHeight);
		entry->init(i, theme);
		entryContainer->add(entry);
		entry->setPosition(0, count * (entryHeight + 8));
		++count;
	}
}
