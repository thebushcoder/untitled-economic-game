/*
 * kingdomComponent.hpp
 *
 *  Created on: 26 Dec 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_KINGDOMCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_KINGDOMCOMPONENT_HPP_

#include <anax/Component.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "serialComponent.hpp"

#include "../kingdomType.hpp"

struct KingdomComponent : anax::Component, SerializableComponent{
private:
	KingdomUtil::KingdomType type;
public:
	KingdomComponent(KingdomUtil::KingdomType t) : type(t){}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		return true;
	}

	KingdomUtil::KingdomType getType() const {
		return type;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_KINGDOMCOMPONENT_HPP_ */
