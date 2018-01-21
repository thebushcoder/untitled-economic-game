/*
 * gameCtrlArea.cpp
 *
 *  Created on: 1 Dec 2016
 *      Author: linbob
 */

#include "gameCtrlArea.hpp"

//included here to avoid circular dependancy error
#include "../../gameScreen.hpp"

void GameCtrlArea::createCtrlArea(GameScreen* screen){
	auto windowWidth = tgui::bindWidth(*screen->getManager()->getGui());
	auto windowHeight = tgui::bindHeight(*screen->getManager()->getGui());

	int buttonWidth = 62, buttonHeight = 28;
	int panelHeight = buttonHeight * 6;
	int speedButtonW = 28;

	auto panel = std::make_shared<tgui::Panel>();
	panel->setSize(buttonWidth * 1.7, panelHeight);
	panel->setPosition(windowWidth - panel->getSize().x,
			windowHeight - panelHeight);

	/*
	 * 	Had to edit to TGUI source to add setAllowFocus
	 */
	panel->setAllowFocus(false);

	tgui::Button::Ptr button = screen->getManager()->getTheme()->load("Button");
	button->setText("Map");
	button->setPosition(speedButtonW + 8, 0 + 4);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [screen]{
		Overworld* o = screen->getOverworld();
		o->setChildActive(!o->isChildActive());
	});
	panel->add(button);

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Save");
	button->setPosition(speedButtonW + 8, buttonHeight * 1 + 8);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [screen]{
		screen->save();
	});
	panel->add(button);

	button = screen->getManager()->getTheme()->load("Button");
	button->setText("Exit");
	button->setPosition(speedButtonW + 8, buttonHeight * 2 + 12);
	button->setSize(buttonWidth, buttonHeight);
	button->connect("pressed", [screen]{
//		screen->save();
		screen->setStatus(QUIT);
		MainMenu* s = new MainMenu();
		s->setManager(screen->getManager());
		screen->getManager()->setCurrentScreen(s);
	});
	panel->add(button);

	tgui::ToggleButton::Ptr toggleButton = screen->getManager()->getTheme()->load("ToggleButton");
	toggleButton->setText("x1");
	toggleButton->setPosition(0 + 4, 0 + 4);
	toggleButton->setSize(speedButtonW, speedButtonW);
	toggleButton->connect("pressed", [screen, toggleButton, panel]{
		if(toggleButton->isChecked()) return false;

		for(auto& w : panel->getWidgets()){
			if(w->getWidgetType().compare("ToggleButton") == 0){
				std::dynamic_pointer_cast<tgui::ToggleButton>(w)->setChecked(false);
			}
		}
		toggleButton->setChecked(true);

		AiSystem::setSpeed(1);

		return true;
	});
	panel->add(toggleButton);

	toggleButton = screen->getManager()->getTheme()->load("ToggleButton");
	toggleButton->setText("x2");
	toggleButton->setPosition(0 + 4, speedButtonW * 1 + 8);
	toggleButton->setSize(speedButtonW, speedButtonW);
	toggleButton->connect("pressed", [screen, toggleButton, panel]{
		if(toggleButton->isChecked()) return false;

		for(auto& w : panel->getWidgets()){
			if(w->getWidgetType().compare("ToggleButton") == 0){
				std::dynamic_pointer_cast<tgui::ToggleButton>(w)->setChecked(false);
			}
		}
		toggleButton->setChecked(true);

		AiSystem::setSpeed(2);

		toggleButton->getParent()->getParent()->unfocusWidgets();
//		toggleButton->getParent()->getParent()->focusPreviousWidget();

		return true;
	});
	panel->add(toggleButton);

	toggleButton = screen->getManager()->getTheme()->load("ToggleButton");
	toggleButton->setText("x3");
	toggleButton->setPosition(0 + 4, speedButtonW * 2 + 12);
	toggleButton->setSize(speedButtonW, speedButtonW);
	toggleButton->connect("pressed", [screen, toggleButton, panel]{
		if(toggleButton->isChecked()) return false;

		for(auto& w : panel->getWidgets()){
			if(w->getWidgetType().compare("ToggleButton") == 0){
				std::dynamic_pointer_cast<tgui::ToggleButton>(w)->setChecked(false);
			}
		}
		toggleButton->setChecked(true);

		AiSystem::setSpeed(3);

		return true;
	});
	panel->add(toggleButton);

	toggleButton = screen->getManager()->getTheme()->load("ToggleButton");
	toggleButton->setText("x4");
	toggleButton->setPosition(0 + 4, speedButtonW * 3 + 16);
	toggleButton->setSize(speedButtonW, speedButtonW);
	toggleButton->connect("pressed", [screen, toggleButton, panel]{
		if(toggleButton->isChecked()) return false;

		for(auto& w : panel->getWidgets()){
			if(w->getWidgetType().compare("ToggleButton") == 0){
				std::dynamic_pointer_cast<tgui::ToggleButton>(w)->setChecked(false);
			}
		}
		toggleButton->setChecked(true);

		AiSystem::setSpeed(4);

		return true;
	});
	panel->add(toggleButton);

	toggleButton = screen->getManager()->getTheme()->load("ToggleButton");
	toggleButton->setText("x5");
	toggleButton->setPosition(0 + 4, speedButtonW * 4 + 20);
	toggleButton->setSize(speedButtonW, speedButtonW);
	toggleButton->connect("pressed", [screen, toggleButton, panel]{
		if(toggleButton->isChecked()) return false;

		for(auto& w : panel->getWidgets()){
			if(w->getWidgetType().compare("ToggleButton") == 0){
				std::dynamic_pointer_cast<tgui::ToggleButton>(w)->setChecked(false);
			}
		}
		toggleButton->setChecked(true);

		AiSystem::setSpeed(5);

		return true;
	});
	panel->add(toggleButton);

	screen->getManager()->getGui()->add(panel, "game_panel");
}
