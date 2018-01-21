/*
 * position.hpp
 *
 *  Created on: 11 Mar 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_POSITIONCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_POSITIONCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

struct PositionComponent : anax::Component, public SerializableComponent{
	sf::Vector2i screenPosition;
	sf::Vector2i tilePosition;

	PositionComponent(){}

	void setPosition(int x, int y, int screenX, int screenY){
		tilePosition.x = x;
		tilePosition.y = y;

		screenPosition.x = screenX;
		screenPosition.y = screenY;
	}

    bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
    	rapidjson::Value t(rapidjson::kObjectType);

    	t.AddMember("tX", tilePosition.x, dAlloc);
		t.AddMember("tY", tilePosition.y, dAlloc);

		t.AddMember("sX", screenPosition.x, dAlloc);
		t.AddMember("sY", screenPosition.y, dAlloc);

    	entData.AddMember("pos", t, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		tilePosition.x = jsonData["tX"].GetInt();
		tilePosition.y = jsonData["tY"].GetInt();

		screenPosition.x = jsonData["sX"].GetInt();
		screenPosition.y = jsonData["sY"].GetInt();
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_POSITIONCOMPONENT_HPP_ */
