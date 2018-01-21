/*
 * mountComponent.hpp
 *
 *  Created on: 21 Jul 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_MOUNTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_MOUNTCOMPONENT_HPP_

#include <anax/Entity.hpp>
#include <anax/Component.hpp>
#include "serialComponent.hpp"

class EntityFactory;

struct MountComponent : anax::Component, SerializableComponent{
	MountComponent(EntityFactory* f, anax::Entity e);

	anax::Entity& getMount(){
		return mount;
	}
	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);
	bool load(rapidjson::Value& jsonData);
private:
	anax::Entity mount;
	EntityFactory* factory = nullptr;
};

#endif /* LIBADO_ENTITY_COMPONENTS_MOUNTCOMPONENT_HPP_ */
