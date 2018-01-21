/*
 * entityPlaceable.hpp
 *
 *  Created on: 29 Mar 2016
 *      Author: linbob
 */

#ifndef ENTITYPLACEABLE_HPP_
#define ENTITYPLACEABLE_HPP_

#include <math.h>
#include <anax/Entity.hpp>

#include "libado/placement.hpp"
#include "libado/map/tileMap.hpp"
#include "libado/entity/entityFactory.hpp"

struct EntityPlaceable : public Placeable{
	static constexpr float MAX_DRAG_TIME = 0.4f;

	EntityPlaceable(EntityFactory* ef, TileMap* m, WorldView* v, sf::RenderWindow* w);
	~EntityPlaceable(){}

	virtual bool place(int tileX, int tileY);
	virtual void render(sf::RenderWindow& window);
	void setSelection(std::string n, std::string s);
	virtual void rotateSelection();
	virtual bool checkConditions();
	virtual void reset();
private:
	void townNamePrompt(anax::Entity e);
protected:
	TileMap* map;
	EntityFactory* factory;

	std::string entityName;
	std::unique_ptr<sf::Sprite> preview;
};

#endif /* ENTITYPLACEABLE_HPP_ */
