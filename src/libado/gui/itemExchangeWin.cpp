/*
 * itemExchangeWin.cpp
 *
 *  Created on: 21 Sep 2017
 *      Author: linbob
 */

#include "itemExchangeWin.hpp"
#include "../../gameScreen.hpp"

void ItemExchangeWindow::init(GameScreen* gs){
	this->theme = gs->getManager()->getTheme();
	this->factory = &gs->getEntFactory();

	connect("closed", [this]{
		this->hide();
	});

	setSize(664, 564);
	setTitle("Trade window");

	int buttonW = 54;
	int buttonH = 28;

	//LEFT LABEL
	tgui::Label::Ptr leftLabel = theme->load("Label");
	leftLabel->setText("Entity: xxx");
	leftLabel->setSize(buttonW * 2, buttonH);
	leftLabel->setPosition(4, 0);
	add(leftLabel, "left_label");

	//RIGHT LABEL
	tgui::Label::Ptr rightLabel = theme->load("Label");
	rightLabel->setText("Player: xxx");
	rightLabel->setSize(buttonW * 2, buttonH);
	rightLabel->setPosition((getSize().x - buttonW * 3) - 4, 0);
	add(rightLabel, "right_label");

	auto leftArea = std::make_shared<ItemArea>(gs);
	leftArea->setPosition(4, buttonH + 4);
	leftArea->setSize(getSize().x * 0.47, getSize().y - (buttonH + 8));
	add(leftArea, "left_area");

	tgui::Button::Ptr button = theme->load("Button");
	button->setText("<");
	button->setSize(buttonH, buttonH);
	float x = leftArea->getPosition().x + leftArea->getSize().x + 2;
	float y = getSize().y * 0.41;
	button->setPosition(x, y);
	add(button, "R_to_L");

	button = theme->load("Button");
	button->setText(">");
	button->setSize(buttonH, buttonH);
	x = leftArea->getPosition().x + leftArea->getSize().x + 2;
	y = getSize().y * 0.51;
	button->setPosition(x, y);
	add(button, "L_to_R");

	auto rightArea = std::make_shared<ItemArea>(gs);
	rightArea->setSize(getSize().x * 0.47, getSize().y - (buttonH + 8));
	rightArea->setPosition(getSize().x - (rightArea->getSize().x + 4), buttonH + 4);
	add(rightArea, "right_area");

	hide();
}
void ItemExchangeWindow::show(anax::Entity e){
	tgui::ChildWindow::show();
}
