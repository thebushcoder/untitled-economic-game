/*
 * provencesComponent.hpp
 *
 *  Created on: 24 Dec 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_PROVINCESCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_PROVINCESCOMPONENT_HPP_

#include <anax/Component.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "serialComponent.hpp"

#include "../../map/voronoi/center.hpp"

struct ProvincesComponent : anax::Component, SerializableComponent{
	std::vector<VoronoiMap::Center*> list;

	ProvincesComponent(){}

	void addProvence(VoronoiMap::Center* c){
		list.push_back(c);
	}
	std::vector<VoronoiMap::Center*>& getList(){
		return list;
	}
	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_PROVINCESCOMPONENT_HPP_ */
