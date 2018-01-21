/*
 * populationComponent.hpp
 *
 *  Created on: 29 Jan 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_POPULATIONCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_POPULATIONCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

struct PopulationComponent : anax::Component, SerializableComponent{
	int value;

	PopulationComponent(){}
	PopulationComponent(int pop) : value(pop){}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		entData.AddMember("pop", value, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		value = jsonData.GetInt();
		return true;
	}
};


#endif /* LIBADO_ENTITY_COMPONENTS_POPULATIONCOMPONENT_HPP_ */
