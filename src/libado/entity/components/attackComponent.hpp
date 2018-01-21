/*
 * DamageComponent.hpp
 *
 *  Created on: 19 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ATTACKCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_ATTACKCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

struct AttackComponent : anax::Component, SerializableComponent{
	int baseValue = 1;

	AttackComponent(){}
	AttackComponent(int attack) : baseValue(attack){}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_ATTACKCOMPONENT_HPP_ */
