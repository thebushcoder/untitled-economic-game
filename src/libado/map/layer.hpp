/*
 * layer.hpp
 *
 *  Created on: 14 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_MAP_LAYER_HPP_
#define LIBADO_MAP_LAYER_HPP_

#include <SFML/Graphics.hpp>

struct Layer{

	Layer(){}
	virtual ~Layer(){}

	virtual void render(sf::RenderWindow* window) = 0;
	virtual void resize(int w, int h) = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual bool isOccupied(int x, int y) = 0;
};



#endif /* LIBADO_MAP_LAYER_HPP_ */
