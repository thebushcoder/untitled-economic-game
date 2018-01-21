/*
 * mountComponent.cpp
 *
 *  Created on: 5 Aug 2017
 *      Author: linbob
 */

#include "mountComponent.hpp"
#include "../entityFactory.hpp"

MountComponent::MountComponent(EntityFactory* f, anax::Entity e) : mount(e), factory(f){}

bool MountComponent::save(rapidjson::Value& entData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
	rapidjson::Value mnt(rapidjson::kObjectType);
	factory->saveEntity(mount, mnt, dAlloc);
	entData.AddMember("mount", mnt, dAlloc);
	return true;
}
bool MountComponent::load(rapidjson::Value& jsonData){
	return true;
}

