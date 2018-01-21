/*
 * cursorOutline.cpp
 *
 *  Created on: 10 Oct 2016
 *      Author: linbob
 */

#include "cursorOutline.hpp"

CursorOutline::CursorOutline(TileMap* map, ThorInput* input) :
	map(map), lastX(0), lastY(0){
	outlines["normal"] = map->getFactory()->getAtlas().createSprite("gui/cursor");
	outlines["green"] = map->getFactory()->getAtlas().createSprite("gui/cursor_green");
	outlines["red"] = map->getFactory()->getAtlas().createSprite("gui/cursor_red");
	outlines["tutorial_cursor"] = map->getFactory()->getAtlas().createSprite("gui/tutorial_cursor");
}
bool CursorOutline::setCurrent(std::string name){
	if(outlines.find(name) == outlines.end()){
		return false;
	}
	current = outlines[name].get();

	return true;
}
void CursorOutline::render(sf::RenderWindow& window){
	if(current != nullptr){
		window.draw(*current);
	}
}
void CursorOutline::mouseMoved(WorldView* w_view, sf::RenderWindow* window){
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2f worldVec = window->mapPixelToCoords(mousePos, *(w_view->view));

	//tile position
	int x = std::round(worldVec.x / TILESIZE);
	int y = std::round(worldVec.y / TILESIZE);

	//check map bounds
	if(x < 0 || y < 0 || x >= map->getWidth() || y >= map->getHeight()) return;

	if(x != lastX || y != lastY){
		lastX = x;
		lastY = y;
		if(current != nullptr){
			current->setPosition(x * TILESIZE, y * TILESIZE);
		}
	}
}
