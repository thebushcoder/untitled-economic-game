/*
 * tilePlaceable.hpp
 *
 *  Created on: 25 Mar 2016
 *      Author: linbob
 */

#ifndef TILEPLACEABLE_HPP_
#define TILEPLACEABLE_HPP_

#include <math.h>

#include "libado/placement.hpp"
#include "libado/map/tileFactory.hpp"
#include "libado/map/tileMap.hpp"

struct TilePlaceable : public Placeable{
	static constexpr float MAX_DRAG_TIME = 0.4f;

	TilePlaceable(TileMap* m, WorldView* v, sf::RenderWindow* w);
	~TilePlaceable(){}

	virtual bool place(int tileX, int tileY);
	virtual void render(sf::RenderWindow& window);
	void setSelection(TileTemplate* t);
	virtual void rotateSelection();
	virtual bool checkConditions();
	virtual void reset();
	void renderOutlines(int x, int y, sf::RenderWindow& window);
protected:
	TileMap* map;

	TileTemplate* tile;
	std::unique_ptr<sf::Sprite> preview;
	std::unique_ptr<sf::Sprite> tileOutline;
};

#endif /* TILEPLACEABLE_HPP_ */
