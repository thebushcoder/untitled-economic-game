/*
 * nameComponent.hpp
 *
 *  Created on: 30 Mar 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_NAMECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_NAMECOMPONENT_HPP_

#include <anax/Component.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "serialComponent.hpp"

struct NameComponent : anax::Component, SerializableComponent{
	std::string name;

	NameComponent(){}
	NameComponent(std::string n) : name(n){}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		rapidjson::Value val(rapidjson::StringRef(name.c_str()));
		entData.AddMember("name", val, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_NAMECOMPONENT_HPP_ */
