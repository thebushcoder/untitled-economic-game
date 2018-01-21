/*
 * cutCopyArea.hpp
 *
 *  Created on: 13 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_AREAEDIT_HPP_
#define LIBADO_AREAEDIT_HPP_

#include <SFML/Window.hpp>

#include "map/tileMap.hpp"
#include "worldView.hpp"
#include "entity/entityFactory.hpp"

struct AreaEdit{
	enum Mode{
		CUT,
		COPY,
		DELETE
	};

	AreaEdit(WorldView* w_view, EntityFactory* entFactory, TileMap* map);
	void setActive(Mode mode, bool active = true);
	bool leftClick(WorldView* w_view, sf::RenderWindow* window);
	bool rightClick();
	void render(sf::RenderWindow& window);
protected:
	void copyArea();
	void cutArea();
	void deleteArea();
private:
	bool isActive = false;
	Mode mode;
	int clickCount = 0;
	TileMap* map;
	EntityFactory* entFactory;
	WorldView* w_view;
	sf::IntRect area;	//src area
	int clickX = 0, clickY = 0;

	std::unique_ptr<sf::Sprite> areaSprite;
};

#endif /* LIBADO_AREAEDIT_HPP_ */
