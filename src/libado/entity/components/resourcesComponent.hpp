/*
 * resourcesComponent.hpp
 *
 *  Created on: 2 Jan 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_RESOURCESCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_RESOURCESCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

//town inventory system
struct ResourcesComponent : anax::Component, SerializableComponent{
	struct Resource{
		anax::Entity e;
		float currentPrice, currentWeeksSupply;
	};

	std::map<std::string, Resource> map; // resource name : <entity, price>

	ResourcesComponent(){}

	Resource& getResource(std::string name){
		return map[name];
	}

	bool hasResource(std::string name){
		return map.find(name) != map.end();
	}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){

		return true;
	}
	bool load(rapidjson::Value& jsonData){

		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_RESOURCESCOMPONENT_HPP_ */
