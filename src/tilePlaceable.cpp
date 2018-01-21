/*
 * tilePlaceable.cpp
 *
 *  Created on: 25 Mar 2016
 *      Author: linbob
 */

#include "tilePlaceable.hpp"

TilePlaceable::TilePlaceable(TileMap* m, WorldView* v, sf::RenderWindow* w)
	:	map(m), tile(nullptr){
	tileOutline = map->getFactory()->getAtlas().createSprite("gui/cursor_green");
}

bool TilePlaceable::place(int tileX, int tileY){
	//if tile is placed outside map bounds, resize and expand map
	if(tileX < 0 || tileY < 0 || tileX >= map->getWidth() ||
			tileY >= map->getHeight()){
		int wDiff = 0, hDiff = 0;
		if(tileX < 0){
			wDiff = std::abs(0 + tileX);
		}else if(tileX >= map->getWidth()){
			wDiff = tileX - map->getWidth();
		}
		if(tileY < 0){
			hDiff = std::abs(0 + tileY);
		}else if(tileY >= map->getHeight()){
			hDiff = tileY - map->getHeight();
		}

		map->resize(wDiff + map->getWidth(), hDiff + map->getHeight());

		return true;
	}

	//place tile
	std::unique_ptr<Tile> t = map->getFactory()->createTile(tile->getId());
	t->setPosition(tileX * TILESIZE, tileY * TILESIZE);
	map->getTileLayer().setTile(tileX, tileY, std::move(t));

	//reset selected tile unless shift if pressed
	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
		reset();
	}
	return true;
}
void TilePlaceable::render(sf::RenderWindow& window){
	if(preview == nullptr) return;

	sf::Vector2f converted = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	int x = std::round(converted.x / TILESIZE);
	int y = std::round(converted.y / TILESIZE);

	renderOutlines(x, y, window);

	preview->setPosition(x * TILESIZE, y * TILESIZE);
	window.draw(*preview);
}
void TilePlaceable::renderOutlines(int x, int y, sf::RenderWindow& window){
	if(x < 0){
		for(int k = -1; k >= x; --k){
			for(int j = 0; j < map->getHeight(); ++j){
				tileOutline->setPosition(k * TILESIZE, j * TILESIZE);
				window.draw(*tileOutline);
			}
		}
	}else if(x >= map->getWidth()){
		for(int k = x; k >= map->getWidth(); --k){
			for(int j = 0; j < map->getHeight(); ++j){
				tileOutline->setPosition(k * TILESIZE, j * TILESIZE);
				window.draw(*tileOutline);
			}
		}
	}
	if(y < 0){
		for(int k = -1; k >= y; --k){
			for(int j = 0; j < map->getWidth(); ++j){
				tileOutline->setPosition(j * TILESIZE, k * TILESIZE);
				window.draw(*tileOutline);
			}
		}
	}else if(y >= map->getHeight()){
		for(int k = y; k >= map->getHeight(); --k){
			for(int j = 0; j < map->getWidth(); ++j){
				tileOutline->setPosition(j * TILESIZE, k * TILESIZE);
				window.draw(*tileOutline);
			}
		}
	}
}
void TilePlaceable::setSelection(TileTemplate* t){
	tile = t;
	preview = map->getFactory()->getAtlas().createSprite(t->getSprite());
}
bool TilePlaceable::checkConditions(){
	return tile != nullptr;
}
void TilePlaceable::rotateSelection(){

}
void TilePlaceable::reset(){
	tile = nullptr;
	preview.release();
}
