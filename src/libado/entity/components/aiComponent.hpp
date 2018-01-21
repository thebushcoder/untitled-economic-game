/*
 * AiComponent.hpp
 *
 *  Created on: 14 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_AICOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_AICOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

#include "../../util/bt-master/bt.hpp"

struct AiComponent : anax::Component, public SerializableComponent{
	std::vector<sf::Vector2i> path;
	bt::Blackboard::Ptr board;
	std::string treeName;

	AiComponent(){
		path = std::vector<sf::Vector2i>();
		board = std::make_shared<bt::Blackboard>();
	}
	AiComponent(std::string n){
		path = std::vector<sf::Vector2i>();
		board = std::make_shared<bt::Blackboard>();
		treeName = n;
	}
	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		return true;
	}
	bool load(rapidjson::Value& jsonData){
//		position = sf::Vector2f(jsonData["x"].GetFloat(), jsonData["y"].GetFloat());
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_AICOMPONENT_HPP_ */
