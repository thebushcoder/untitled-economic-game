/*
 * healthComponent.hpp
 *
 *  Created on: 19 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_HEALTHCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_HEALTHCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

struct HealthComponent : anax::Component, SerializableComponent{
	int maxValue = 1;
	int currentValue = 1;

	HealthComponent(){}
	HealthComponent(int hp) : maxValue(hp), currentValue(hp){}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		rapidjson::Value t(rapidjson::kObjectType);

		t.AddMember("current", currentValue, dAlloc);
		t.AddMember("max", maxValue, dAlloc);

		entData.AddMember("hp", t, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		currentValue = jsonData["current"].GetInt();
		maxValue = jsonData["max"].GetInt();
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_HEALTHCOMPONENT_HPP_ */
