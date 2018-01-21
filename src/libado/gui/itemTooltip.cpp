/*
 * itemTooltip.cpp
 *
 *  Created on: 1 Jul 2017
 *      Author: linbob
 */

#include "itemTooltip.hpp"

ItemTooltip::ItemTooltip(tgui::Theme::Ptr theme){
	setBackgroundColor(tgui::Color(0, 51, 204, 198));
	getRenderer()->setBorderColor(tgui::Color(255, 255, 255));
	getRenderer()->setBorders(2, 2);

	setSize(242, 298);

	int entryH = 32;

	tgui::Panel::Ptr nameEntry = std::make_shared<tgui::Panel>();
	nameEntry->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	nameEntry->setSize(getSize().x * 0.96, entryH);

	//sprite
	SpriteWidget::Ptr sprite = std::make_shared<SpriteWidget>();
	sprite->setSize(24, 24);
	sprite->setPosition(2, 2);
	nameEntry->add(sprite, "item_sprite");

	//name
	tgui::Label::Ptr name = theme->load("Label");
	name->setTextSize(16);
	name->setSize(getSize().x - (sprite->getSize().x + 6), entryH);
	name->setPosition(sprite->getSize().x * 2, 2);
	nameEntry->add(name, "item_name");

	widgets["name_entry"] = nameEntry;

	//weight
	tgui::Panel::Ptr weightEntry = std::make_shared<tgui::Panel>();
	weightEntry->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	weightEntry->setSize(getSize().x * 0.96, entryH);

	tgui::Label::Ptr weightLabel = theme->load("Label");
	weightLabel->setText("Weight: ");
	weightLabel->setSize(getSize().x * 0.48, entryH);
	weightEntry->add(weightLabel, "value_weight");

	tgui::Label::Ptr weight = theme->load("Label");
	weight->setSize(getSize().x * 0.48, entryH);
	weight->setPosition(weightLabel->getSize().x + 4, 0);
	weightEntry->add(weight, "item_weight");

	widgets["weight_entry"] = weightEntry;

	//value
	tgui::Panel::Ptr valueEntry = std::make_shared<tgui::Panel>();
	valueEntry->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	valueEntry->setSize(getSize().x * 0.96, entryH);

	tgui::Label::Ptr valueLabel = theme->load("Label");
	valueLabel->setText("Value: ");
	valueLabel->setSize(getSize().x * 0.48, entryH);
	valueEntry->add(valueLabel, "value_label");

	tgui::Label::Ptr value = theme->load("Label");
	value->setSize(getSize().x * 0.48, entryH);
	value->setPosition(valueLabel->getSize().x + 4, 0);
	valueEntry->add(value, "item_value");

	widgets["value_entry"] = valueEntry;

	//type
	tgui::Panel::Ptr typeEntry = std::make_shared<tgui::Panel>();
	typeEntry->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	typeEntry->setSize(getSize().x * 0.96, entryH);

	tgui::Label::Ptr typeLabel = theme->load("Label");
	typeLabel->setText("Type: ");
	typeLabel->setSize(getSize().x * 0.48, entryH);
	typeEntry->add(typeLabel, "type_label");

	tgui::Label::Ptr type = theme->load("Label");
	type->setTextSize(12);
	type->setSize(getSize().x * 0.48, entryH);
	type->setPosition(typeLabel->getSize().x + 4, 0);
	typeEntry->add(type, "item_type");

	widgets["type_entry"] = typeEntry;

	//desc
	tgui::Panel::Ptr descEntry = std::make_shared<tgui::Panel>();
	descEntry->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	descEntry->setSize(getSize().x * 0.96, entryH * 4);

	tgui::Label::Ptr descLabel = theme->load("Label");
	descLabel->setText("Description: ");
	descLabel->setSize(getSize().x - 4, entryH);
	descEntry->add(descLabel, "desc_label");

	tgui::Label::Ptr desc = theme->load("Label");
	desc->setSize(getSize().x - 4, entryH * 3);
	desc->setPosition(0, descLabel->getPosition().y + descLabel->getSize().y + 4);
	descEntry->add(desc, "item_desc");

	widgets["desc_entry"] = descEntry;

	//bonus'/effects

}

void ItemTooltip::updateContents(anax::Entity& item){
	removeAllWidgets();

	tgui::Panel::Ptr nameEntry = widgets["name_entry"];

	SpriteComponent& spr = item.getComponent<SpriteComponent>();

	nameEntry->get<SpriteWidget>("item_sprite")->setSprite(spr.sprite.get());

	NameComponent& n = item.getComponent<NameComponent>();
	tgui::Label::Ptr name = nameEntry->get<tgui::Label>("item_name");
	name->setText(n.name);

	add(nameEntry, "name_entry");

	tgui::Widget::Ptr lastEntry = nameEntry;

	if(item.hasComponent<ValueComponent>()){
		ValueComponent& v = item.getComponent<ValueComponent>();

		tgui::Panel::Ptr e = widgets["value_entry"];
		e->get<tgui::Label>("item_value")->setText(std::to_string(v.getValue()));
		e->setPosition(4, lastEntry->getPosition().y + lastEntry->getSize().y + 4);
		add(e, "value_entry");

		lastEntry = e;
	}
	if(item.hasComponent<WeightComponent>()){
		WeightComponent& w = item.getComponent<WeightComponent>();

		tgui::Panel::Ptr e = widgets["weight_entry"];
		e->get<tgui::Label>("item_weight")->setText(std::to_string(w.getWeight()));
		e->setPosition(4, lastEntry->getPosition().y + lastEntry->getSize().y + 4);
		add(e, "weight_entry");

		lastEntry = e;
	}
	if(item.hasComponent<ItemComponent>()){
		ItemComponent& t = item.getComponent<ItemComponent>();

		tgui::Panel::Ptr e = widgets["type_entry"];
		std::string typeString = "";
		for(auto& j : t.types){
			typeString += Item::enumToString(j) + ", ";
		}
		e->get<tgui::Label>("item_type")->setText(typeString);
		e->setPosition(4, lastEntry->getPosition().y + lastEntry->getSize().y + 4);
		add(e, "type_entry");

		lastEntry = e;
	}
	if(item.hasComponent<DescriptionComponent>()){
		DescriptionComponent& d = item.getComponent<DescriptionComponent>();

		tgui::Panel::Ptr e = widgets["desc_entry"];
		e->get<tgui::Label>("item_desc")->setText(d.getDescription());
		e->setPosition(4, lastEntry->getPosition().y + lastEntry->getSize().y + 4);
		add(e, "desc_entry");

		lastEntry = e;
	}
}
