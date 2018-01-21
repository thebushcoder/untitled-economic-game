/*
 * townResourceWindow.cpp
 *
 *  Created on: 7 Feb 2017
 *      Author: linbob
 */

#include "townResourceWindow.hpp"

void TownResourceWindow::scrollPanel(tgui::ChildWindow* panel, tgui::Scrollbar::Ptr scroll)
{
	static int previousScrolbarValue;
    int distanceToMove = previousScrolbarValue - scroll->getValue();

    // Move all widgets that are inside the panel
    for (auto& widget : panel->getWidgets()){
    	if(((tgui::Scrollbar*)widget.get()) == scroll.get()) continue;
        widget->setPosition(widget->getPosition().x, widget->getPosition().y + distanceToMove);
    }

    previousScrolbarValue = scroll->getValue();
}

void TownResourceWindow::init(TownResourceSys* resSys, tgui::Theme::Ptr t){
	this->resSystem = resSys;
	this->theme = t;

	connect("closed", [this]{
		this->hide();
	});

	setSize(368, 446);

	Container::setAllowFocus(false);
	setName("TownResourceWindow");

	hide();
}
void TownResourceWindow::show(){
	removeAllWidgets();
	int rowCount = 0;

	for(anax::Entity e : resSystem->getEntities()){
		int townRows = 0;
		ResourcesComponent& resComp = e.getComponent<ResourcesComponent>();

		tgui::Panel::Ptr townPanel = theme->load("Panel");
		townPanel->setAllowFocus(false);

		tgui::Label::Ptr nameLabel = std::make_shared<tgui::Label>();
		nameLabel->setTextColor(tgui::Color(255, 255, 255));
		nameLabel->setTextSize(18);
		nameLabel->setSize(getSize().x * 0.6, labelH * 0.8);
		townPanel->add(nameLabel, "town_name");

		NameComponent& n = e.getComponent<NameComponent>();
		nameLabel->setText(n.name);

		++rowCount;
		++townRows;

		for(auto& r : resComp.map){
			addEntry(r.second, townPanel, townRows);
			++townRows;
			++rowCount;
		}
		townPanel->setSize(getSize().x, (labelH + 2) * townPanel->getWidgets().size());
		add(townPanel, n.name);
	}
	updateEntryPositions();

	tgui::Scrollbar::Ptr scrollbar = theme->load("Scrollbar");
	scrollbar->setSize(12, getSize().y);
	scrollbar->setArrowScrollAmount(60);
	scrollbar->setLowValue(getSize().y); // Viewable area (height of the panel)
	float h = labelH;
	for(auto& w : getWidgets()){
		h += w->getSize().y;
	}
	scrollbar->setMaximum(h);
	add(scrollbar, "scroll");
	scrollbar->setPosition(getSize().x - scrollbar->getSize().x, 0);

	scrollbar->connect("ValueChanged", scrollPanel, this, scrollbar);

	setPosition(200, 200);

	ChildWindow::show();
}
void TownResourceWindow::update(sf::Time elapsedTime){
	for(anax::Entity e : resSystem->getEntities()){
		NameComponent& n = e.getComponent<NameComponent>();
		ResourcesComponent& resComp = e.getComponent<ResourcesComponent>();

		tgui::Panel::Ptr townPanel = get<tgui::Panel>(n.name);
		for(auto& r : resComp.map){
			if(tgui::Panel::Ptr p = townPanel->get<tgui::Panel>(r.first)){
				//update existing res entry
				StackableComponent& stack = r.second.e.getComponent<StackableComponent>();
				std::stringstream ss;
				ss << std::fixed << std::setprecision(2) << stack.count;

				p->get<tgui::Label>("item_qty_")->setText(ss.str());

				ss.str("");
				ss << std::fixed << std::setprecision(1) << r.second.currentWeeksSupply;
				p->get<tgui::Label>("item_supply_")->setText(ss.str() + " wks");
				ss.str("");
				ss << std::fixed << std::setprecision(2) << r.second.currentPrice;
				p->get<tgui::Label>("item_price_")->setText("$" + ss.str());

			}else{
				//add new resource entry
				addEntry(r.second, townPanel, townPanel->getWidgets().size());
				//resize town panel
				townPanel->setSize(getSize().x, labelH * townPanel->getWidgets().size());
				updateEntryPositions();
			}
		}
	}

	static int c;

	if(c >= 10){
		c = 0;

//		printf("\n( %s ) isFocused : %s\n", getName().c_str(),isFocused() ? "true" : "false");
//		printf("Window parent( %s )  isFocused : %s\n", getParent()->getName().c_str(), getParent()->isFocused() ? "true" : "false");
//		printf("Window parent's parent isFocused : %s\n", getParent()->getParent()->isFocused() ? "true" : "false");

	}else{
		++c;
	}
}
void TownResourceWindow::updateEntryPositions(){
	for(int i = 0 ; i < getWidgets().size(); ++i){
		float x = 6;
		float y = 2;

		for(int j = 0; j < i; ++j){
			y += getWidgets()[j]->getSize().y;
		}

		getWidgets()[i]->setPosition(x, y);
	}
}
void TownResourceWindow::addEntry(ResourcesComponent::Resource& res, tgui::Panel::Ptr townPanel, int row){
	tgui::Panel::Ptr p = theme->load("Panel");
	p->setAllowFocus(false);
	p->setSize(getSize().x, labelH);
	p->setPosition(0, (labelH + 2) * row);

	//SPRITE
	SpriteWidget::Ptr sprite = std::make_shared<SpriteWidget>();
	p->add(sprite, "item_sprite_");

	SpriteComponent& s = res.e.getComponent<SpriteComponent>();

	sprite->setSprite(s.sprite.get());
	sprite->getSprite()->setScale(1.2, 1.2);
	sprite->setPosition(0, 0);

	//NAME
	tgui::Label::Ptr nameLabel = std::make_shared<tgui::Label>();
	nameLabel->setTextColor(fontColor);
	nameLabel->setTextSize(16);
	nameLabel->setSize(getSize().x * 0.6, labelH * 0.8);
	nameLabel->setPosition(getSize().x * 0.12, 0);
	p->add(nameLabel, "item_name_");

	NameComponent& n = res.e.getComponent<NameComponent>();
	nameLabel->setText(n.name);

	//QUANTITY
	tgui::Label::Ptr qtyLabel = std::make_shared<tgui::Label>();
	qtyLabel->setTextColor(fontColor);
	qtyLabel->setTextSize(16);
	qtyLabel->setSize(getSize().x * 0.6, labelH * 0.8);
	qtyLabel->setPosition(getSize().x * 0.35, 0);
	p->add(qtyLabel, "item_qty_");

	StackableComponent& stack = res.e.getComponent<StackableComponent>();
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << stack.count;
	qtyLabel->setText(ss.str());

	//WEEKS SUPPLY
	tgui::Label::Ptr supplyLabel = std::make_shared<tgui::Label>();
	supplyLabel->setTextColor(fontColor);
	supplyLabel->setTextSize(16);
	supplyLabel->setSize(getSize().x * 0.6, labelH * 0.8);
	supplyLabel->setPosition(getSize().x * 0.59, 0);
	p->add(supplyLabel, "item_supply_");

	ss.str("");
	ss << std::fixed << std::setprecision(1) << res.currentWeeksSupply;
	supplyLabel->setText(ss.str() + " wks");

	//PRICE
	tgui::Label::Ptr priceLabel = std::make_shared<tgui::Label>();
	priceLabel->setTextColor(fontColor);
	priceLabel->setTextSize(16);
	priceLabel->setSize(getSize().x * 0.6, labelH * 0.8);
	priceLabel->setPosition(getSize().x * 0.78, 0);
	p->add(priceLabel, "item_price_");

	std::stringstream ps;
	ps << std::fixed << std::setprecision(2) << res.currentPrice;
	priceLabel->setText("$" + ps.str());

	townPanel->add(p, n.name);
}
