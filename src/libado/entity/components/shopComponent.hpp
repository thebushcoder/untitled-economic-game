/*
 * shopComponent.hpp
 *
 *  Created on: 14 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_SHOPCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_SHOPCOMPONENT_HPP_

#include <anax/Component.hpp>

struct ShopComponent : anax::Component{
	std::vector<Item::Type> restrictions;
	//Loot list object

	ShopComponent(){}
	ShopComponent(rapidjson::Value& jsonData){

	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_SHOPCOMPONENT_HPP_ */
