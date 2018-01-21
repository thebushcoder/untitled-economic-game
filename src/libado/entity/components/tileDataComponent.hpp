/*
 * tileDataComponent.hpp
 *
 *  Created on: 13 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_TILEDATACOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_TILEDATACOMPONENT_HPP_

#include <anax/Component.hpp>
#include <rapidjson/document.h>

struct TileDataComponent : anax::Component{
	int cost, priority;

	TileDataComponent(){}
	TileDataComponent(int c, int p) : cost(c), priority(p){}
};


#endif /* LIBADO_ENTITY_COMPONENTS_TILEDATACOMPONENT_HPP_ */
