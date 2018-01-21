/*
 * lootList.hpp
 *
 *  Created on: 27 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_LOOTLISTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_LOOTLISTCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

#include "../lootList.hpp"

struct LootListComponent : anax::Component, public SerializableComponent{
	LootList list;

	LootListComponent(rapidjson::Value& json){
		list.load(json);
	}

    bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_LOOTLISTCOMPONENT_HPP_ */
