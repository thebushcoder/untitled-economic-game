/*
 * itemComponent.hpp
 *
 *  Created on: 26 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ITEMCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_ITEMCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

#include "itemTypes.hpp"

/*	Used to tag items
 * 	tag only class
 */
struct ItemComponent : anax::Component, SerializableComponent{
	bool inInventory = false;
	std::vector<Item::Type> types;

	ItemComponent(rapidjson::Value& jsonData){
		for (auto& i : jsonData.GetArray()){
			types.push_back(Item::Type(i.GetInt()));
		}
	}

	bool hasType(Item::Type type){
		for(Item::Type t : types){
			if(type == t){
				return true;
			}
		}
		return false;
	}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		entData.AddMember("item", inInventory, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		inInventory = jsonData.GetBool();
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_ITEMCOMPONENT_HPP_ */
