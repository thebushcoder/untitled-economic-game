/*
 * portalComponent.hpp
 *
 *  Created on: 20 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_PORTALCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_PORTALCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

/*
 * for doors/level change portals
 */
struct PortalComponent : anax::Component, public SerializableComponent{
	std::string zoneId;
	std::string destZoneName;

	PortalComponent(){}
	PortalComponent(std::string name) : destZoneName(name){}

    bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
    	rapidjson::Value t(rapidjson::kObjectType);
    	t.AddMember("id", rapidjson::Value(zoneId.c_str(), zoneId.length()), dAlloc);
    	t.AddMember("name", rapidjson::Value(destZoneName.c_str(), destZoneName.length()), dAlloc);
    	entData.AddMember("portal", t, dAlloc);
		return true;
	}
	bool load(rapidjson::Value& jsonData){
		zoneId = jsonData["id"].GetString();
		destZoneName = jsonData["name"].GetString();
		return true;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_PORTALCOMPONENT_HPP_ */
