/*
 * EventListComponent.hpp
 *
 *  Created on: 14 Mar 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_EVENTLISTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_EVENTLISTCOMPONENT_HPP_

#include <anax/Component.hpp>

#include "serialComponent.hpp"
#include "../systems/randEvent.hpp"

struct EventListComponent : anax::Component, SerializableComponent{
	std::vector<std::pair<RandEvent, int>> pastEvents; //past event, cooldown timer
	std::vector<std::pair<RandEvent, int>> currentEvents; //current event, duration(turns) counter

	std::vector<std::pair<std::string, float>> eventModifiers; //town specific event chance modifiers

	EventListComponent(){
		pastEvents = std::vector<std::pair<RandEvent, int>>();
		currentEvents = std::vector<std::pair<RandEvent, int>>();

		eventModifiers = std::vector<std::pair<std::string, float>>();
	}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){

		return true;
	}
	bool load(rapidjson::Value& jsonData){

		return true;
	}
};



#endif /* LIBADO_ENTITY_COMPONENTS_EVENTLISTCOMPONENT_HPP_ */
