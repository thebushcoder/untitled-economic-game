/*
 * mainMenu.hpp
 *
 *  Created on: 19 Feb 2016
 *      Author: linbob
 */

#ifndef MAINMENU_HPP_
#define MAINMENU_HPP_

#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "libado/screen.hpp"
#include "libado/screenManager.hpp"
#include "libado/gui/saveFileWindow.hpp"
#include "gameScreen.hpp"
#include "test/voronoiScreen.hpp"

class MainMenu : public Screen{
public:
	~MainMenu(){}

	void init();
	void createMainControls();
	void createGameButtons();
	int run();
	void update(sf::Time& delta);
	void render();
	void resize(sf::Event& e);
	void handleInput(sf::Time& delta);
	void loadResources();

protected:
//	sfg::Box::Ptr mainLayout, gameLayout;
};

#endif /* MAINMENU_HPP_ */
