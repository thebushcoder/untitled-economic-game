/*
 * placement.hpp
 *
 *  Created on: 25 Mar 2016
 *      Author: linbob
 */

#ifndef LIBADO_PLACEMENT_HPP_
#define LIBADO_PLACEMENT_HPP_

#include <string>
#include <map>
#include <memory>
#include <SFML/Window.hpp>

#include "input.hpp"
#include "worldView.hpp"
#include "map/tileMap.hpp"

struct Placeable{
	virtual ~Placeable(){}

	virtual void render(sf::RenderWindow& window) = 0;
	virtual void setSelection(){}
	virtual bool place(int tileX, int tileY) = 0;
	virtual bool checkConditions() = 0;
	virtual void rotateSelection() = 0;
	virtual void reset() = 0;
};

class PlacementManager{
public:
	PlacementManager(WorldView* v, sf::RenderWindow* w);

	bool leftClick(WorldView* v, sf::RenderWindow* w);
	bool rightClick();
	bool mouseMoved(WorldView* v, sf::RenderWindow* w);
	void addPlaceable(std::string name, std::unique_ptr<Placeable> p);
	Placeable* getCurrent();
	void setCurrent(std::string name);
	Placeable* getPlaceable(std::string name);
	void reset();
	void render(sf::RenderWindow& window);
protected:
	std::map<std::string, std::unique_ptr<Placeable> > list;
	Placeable* current = nullptr;
	sf::Vector2i lastPlaced;
};



#endif /* LIBADO_PLACEMENT_HPP_ */
