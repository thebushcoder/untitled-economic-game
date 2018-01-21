/*
 * stackableComponent.hpp
 *
 *  Created on: 22 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_STACKABLECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_STACKABLECOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

/*	Used to tag items
 * 	tag only class
 */
struct StackableComponent : anax::Component, SerializableComponent{
	float count = 1;

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		entData.AddMember("stackable", count, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		count = jsonData.GetFloat();
		return true;
	}
};


#endif /* LIBADO_ENTITY_COMPONENTS_STACKABLECOMPONENT_HPP_ */
