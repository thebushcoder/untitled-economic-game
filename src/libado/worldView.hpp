/*
 * worldView.hpp
 *
 *  Created on: 11 Feb 2016
 *      Author: linbob
 */

#ifndef WORLDVIEW_HPP_
#define WORLDVIEW_HPP_

#define CAMERAVELOCITY 600

#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "input.hpp"
//#include "map/zone.hpp"

class Zone;

class WorldView{
public:
	std::unique_ptr<sf::View> view;
	Zone* z = nullptr;

	WorldView(){}
	WorldView(int w, int h, ThorInput& input);
	WorldView(int w, int h, std::string inputExt, ThorInput& input, Zone* z);
	void resize(int w, int h);
	const sf::View* getView();

	WorldView& operator = (WorldView& other);
};

#endif /* WORLDVIEW_HPP_ */
