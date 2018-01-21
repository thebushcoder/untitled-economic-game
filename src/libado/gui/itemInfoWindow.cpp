/*
 * itemInfoWindow.cpp
 *
 *  Created on: 11 Nov 2016
 *      Author: linbob
 */

#include "itemInfoWindow.hpp"

void ItemInfoWindow::init(tgui::Theme::Ptr theme){
	this->theme = theme;

	connect("closed", [this]{
		this->hide();
	});

	setSize(260, 220);

	tgui::Color fontColor(255, 255, 255);

	int labelH = 32;

	SpriteWidget::Ptr sprite = std::make_shared<SpriteWidget>();
	sprite->setPosition(0, 0);
	add(sprite, "item_sprite");

	tgui::Label::Ptr nameLabel = std::make_shared<tgui::Label>();
	nameLabel->setTextColor(fontColor);
	nameLabel->getRenderer()->setBorderColor(fontColor);
	nameLabel->getRenderer()->setBorders(0, 0, 0, 1);
	nameLabel->setTextSize(14);
	nameLabel->setSize(getSize().x * 0.6, labelH * 0.8);
	nameLabel->setPosition(getSize().x * 0.35, 4);
	add(nameLabel, "item_name");

	tgui::Label::Ptr weightLabel = std::make_shared<tgui::Label>();
	weightLabel->setTextColor(fontColor);
	weightLabel->setTextSize(14);
	weightLabel->setSize(getSize().x * 0.2, labelH * 1.2);
	weightLabel->setPosition(getSize().x * 0.35, labelH * 1.1);
	weightLabel->hide();
	add(weightLabel, "item_weight");

	tgui::Label::Ptr valueLabel = std::make_shared<tgui::Label>();
	valueLabel->setTextColor(fontColor);
	valueLabel->setTextSize(14);
	valueLabel->setSize(getSize().x * 0.2, labelH * 1.2);
	valueLabel->setPosition(getSize().x * 0.38 + weightLabel->getSize().x, labelH * 1.1);
	add(valueLabel, "item_value");

	tgui::Label::Ptr typeLabel = std::make_shared<tgui::Label>();
	typeLabel->setTextColor(fontColor);
	typeLabel->setTextSize(14);
	typeLabel->setSize(getSize().x * 0.6, labelH * 1.8);
	typeLabel->setPosition(getSize().x * 0.35, labelH * 2.3);
	add(typeLabel, "item_type");

	tgui::Label::Ptr descLabel = std::make_shared<tgui::Label>();
	descLabel->setTextColor(fontColor);
	descLabel->getRenderer()->setBorderColor(fontColor);
	descLabel->getRenderer()->setBorders(0, 1, 0, 0);
	descLabel->setTextSize(14);
	descLabel->setSize(getSize().x * 0.9, getSize().y * 0.6);
	descLabel->setPosition(getSize().x * 0.05, getSize().y * 0.52);
	add(descLabel, "item_desc");

	hide();
}
void ItemInfoWindow::show(anax::Entity item){
	//conditional item type info fields(weapon = dmg, speed; armour = def; )
	SpriteComponent& s = item.getComponent<SpriteComponent>();
	SpriteWidget::Ptr sprite = get<SpriteWidget>("item_sprite");
	sprite->setSprite(s.sprite.get());
	sprite->getSprite()->setScale(2, 2);

	NameComponent& n = item.getComponent<NameComponent>();
	tgui::Label::Ptr name = get<tgui::Label>("item_name");
	name->setText(n.name);

	ValueComponent& v = item.getComponent<ValueComponent>();
	tgui::Label::Ptr value = get<tgui::Label>("item_value");
	std::stringstream ss;
	ss << v.getValue();
	value->setText("Value:\n" + ss.str());

	if(item.hasComponent<WeightComponent>()){
		WeightComponent& w = item.getComponent<WeightComponent>();
		tgui::Label::Ptr weight = get<tgui::Label>("item_weight");
		std::stringstream ss;
		ss << w.getWeight();
		weight->setText("Weight:\n" + ss.str());
		weight->show();
	}

	DescriptionComponent& d = item.getComponent<DescriptionComponent>();
	tgui::Label::Ptr desc = get<tgui::Label>("item_desc");
	desc->setText(d.getDescription());

	ItemComponent& i = item.getComponent<ItemComponent>();
	tgui::Label::Ptr type = get<tgui::Label>("item_type");
	std::string typeStr = "Type: ";
	for(int x = 0; x < i.types.size(); ++x){
		typeStr += Item::enumToString(i.types[x]);
		if(x == 0){
			typeStr += ",\n";
		}else if(x < i.types.size() - 1){
			typeStr += ", ";
		}
	}
	type->setText(typeStr);

	tgui::ChildWindow::show();
}
void ItemInfoWindow::hide(){
	tgui::Label::Ptr weight = get<tgui::Label>("item_weight");
	weight->hide();
	tgui::ChildWindow::hide();
}
