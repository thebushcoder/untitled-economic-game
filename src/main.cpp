/*
 * main.cpp
 *
 *  Created on: 8 Feb 2016
 *      Author: linbob
 */

#include "main.hpp"

int main(){
	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "My window");

	std::shared_ptr<ScreenManager> screenMan = std::make_shared<ScreenManager>(
			"bin/font/CaviarDreams_Bold.ttf", window.get());

	auto windowWidth = tgui::bindWidth(*screenMan->getGui());
	auto windowHeight = tgui::bindHeight(*screenMan->getGui());

	Screen* menu = new MainMenu();
	menu->setManager(screenMan.get());
	screenMan->setCurrentScreen(menu);

	screenMan->run();

	screenMan->getWindow()->close();

    return 0;
}


