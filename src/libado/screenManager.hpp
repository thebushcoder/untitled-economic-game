/*
 * screenManager.hpp
 *
 *  Created on: 15 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_SCREENMANAGER_HPP_
#define LIBADO_SCREENMANAGER_HPP_

#include <string>
#include <map>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "screen.hpp"
#include "gui/itemArea.hpp"
#include "gui/spriteButton.hpp"
#include "gui/lootWindow.hpp"
#include "gui/inventoryWindow.hpp"
#include "gui/tooltip.hpp"
#include "gui/itemInfoWindow.hpp"
#include "gui/shopWindow.hpp"
#include "gui/townResourceWindow.hpp"
#include "gui/toggleButton.hpp"
#include "gui/mountInvenWin.hpp"

class ScreenManager{
public:
	ScreenManager(std::string fontPath, sf::RenderWindow* w) :
		current(nullptr){
		window = w;
		gui = std::make_shared<tgui::Gui>(*window);

		font = std::make_shared<sf::Font>();
		font->loadFromFile("bin/font/CaviarDreams_Bold.ttf");
		gui->setFont(tgui::Font(font));
		gui->getContainer()->setName("GUI_container");

		theme = std::make_shared<tgui::Theme>("bin/imgs/Black.txt");
		theme->setConstructFunction("SpriteButton", std::make_shared<SpriteButton>);
		theme->setConstructFunction("CustomTooltip", std::make_shared<tgui::Label>);
		theme->setConstructFunction("LootWindow", std::make_shared<LootWindow>);
		theme->setConstructFunction("ItemInfoWindow", std::make_shared<ItemInfoWindow>);
		theme->setConstructFunction("InventoryWindow", std::make_shared<InventoryWindow>);
		theme->setConstructFunction("ItemEntry", std::make_shared<ItemEntry>);
		theme->setConstructFunction("ConfirmDialog", std::make_shared<ConfirmDialog>);
		theme->setConstructFunction("ShopWindow", std::make_shared<ShopWindow>);
		theme->setConstructFunction("MountInvenWindow", std::make_shared<MountInvenWindow>);
		theme->setConstructFunction("TownResWin", std::make_shared<TownResourceWindow>);
		theme->setConstructFunction("ToggleButton", std::make_shared<tgui::ToggleButton>);
	}
	~ScreenManager(){}

	Screen* getCurrentScreen(){
		return current.get();
	}
	void resize(sf::Event& e){
//		for(auto const &s : screens) {
//			if(s.second != current && s.second->getStatus() != UNINIT){
//				s.second->resize(e);
//			}
//		}
	}
	void setCurrentScreen(Screen* s){
		if(s){
			if(!current){
				current = std::unique_ptr<Screen>(std::move(s));
			}else if(!next){
				next = std::unique_ptr<Screen>(std::move(s));
			}else{
				next.reset(std::move(s));
			}
		}else{
			current = NULL;
		}
		gui->removeAllWidgets();
	}
	void run(){
		while(current){
			if(current->getStatus() == QUIT){
				if(!next) return;
				current.reset(next.release());
			}else{
				current->run();
			}
		}
	}
	tgui::Theme::Ptr getTheme(){
		return theme;
	}
	tgui::Gui* getGui(){
		return gui.get();
	}
	sf::RenderWindow* getWindow(){
		return window;
	}
protected:
	std::unique_ptr<Screen> next;
	std::unique_ptr<Screen> current;

	sf::RenderWindow* window;

	std::shared_ptr<sf::Font> font;
	tgui::Theme::Ptr theme;

	std::shared_ptr<tgui::Gui> gui;
};

#endif /* LIBADO_SCREENMANAGER_HPP_ */

