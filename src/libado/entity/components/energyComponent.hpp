/*
 * energyHeader.hpp
 *
 *  Created on: 16 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ENERGYCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_ENERGYCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

struct EnergyComponent : anax::Component, SerializableComponent{
	int energyRecovery = 0;
	int count = 100;

	EnergyComponent(){}
	EnergyComponent(int baseRec) : energyRecovery(baseRec){}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		entData.AddMember("energy", count, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		count = jsonData.GetInt();
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_ENERGYCOMPONENT_HPP_ */
