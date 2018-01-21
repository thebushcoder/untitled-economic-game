/*
 * cursorOutline.hpp
 *
 *  Created on: 10 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_CURSOROUTLINE_HPP_
#define LIBADO_CURSOROUTLINE_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <memory>

#include "map/tileMap.hpp"
#include "worldView.hpp"
#include "input.hpp"

class CursorOutline{
public:
	CursorOutline(TileMap* map, ThorInput* input);
	bool setCurrent(std::string name);
	void render(sf::RenderWindow& window);
	void mouseMoved(WorldView* w_view, sf::RenderWindow* window);
private:
	TileMap* map;
	int lastX, lastY;
	std::map<std::string, std::unique_ptr<sf::Sprite>> outlines;
	sf::Sprite* current = nullptr;
};



#endif /* LIBADO_CURSOROUTLINE_HPP_ */
