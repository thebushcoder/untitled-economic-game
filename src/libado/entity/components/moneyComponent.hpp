/*
 * MoneyComponent.hpp
 *
 *  Created on: 18 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_MONEYCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_MONEYCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

struct MoneyComponent : anax::Component, SerializableComponent{
	int count = 0;

	MoneyComponent(){}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		entData.AddMember("money", count, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		count = jsonData.GetInt();
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_MONEYCOMPONENT_HPP_ */
