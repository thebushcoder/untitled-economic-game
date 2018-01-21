/*
 * equipmentComponent.cpp
 *
 *  Created on: 5 Aug 2017
 *      Author: linbob
 */

#include "equipmentComponent.hpp"
#include "../entityFactory.hpp"

EquipmentComponent::EquipmentComponent(EntityFactory* f, rapidjson::Value& jsonData) : factory(f){
	for(auto& v : jsonData.GetArray()){
		if(v.IsArray()){
			Item::Type type = (Item::Type)v[0].GetInt();

			for(int i = 1; i < v.GetArray().Size(); ++i){
				list.push_back(std::make_shared<EquipmentSlot>(type, i));
			}
		}else{
			Item::Type type = (Item::Type)v.GetInt();

			list.push_back(std::make_shared<EquipmentSlot>(type));
		}
	}
}

EquipmentSlot* EquipmentComponent::getSlot(Item::Type type, int slotId){
	for(auto s : list){
		if(s->getSlotType() == type && s->getId() == slotId){
			return s.get();
		}
	}

	return nullptr;
}

bool EquipmentComponent::save(rapidjson::Value& entData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
	rapidjson::Value slots(rapidjson::kArrayType);

	for(auto es : list) {
		if(!es->hasItem()) continue;

		rapidjson::Value s(rapidjson::kObjectType);
		s.AddMember("type", es->type, dAlloc);
		s.AddMember("id", es->id, dAlloc);

		anax::Entity item = factory->world->getEntity(es->getSlotItem());
		factory->saveEntity(item, s, dAlloc);

		slots.PushBack(s, dAlloc);
	}

	entData.AddMember("equip", slots, dAlloc);

	return true;
}
bool EquipmentComponent::load(rapidjson::Value& jsonData){
	for (auto& i : jsonData.GetArray()){
		EquipmentSlot* s = getSlot(Item::Type(i["type"].GetInt()), i["id"].GetInt());
		std::string name = i["entity"]["name"].GetString();
		anax::Entity e = factory->createEntity(name);

		factory->loadEntity(e, i["entity"]);

		s->equipItem(e);
	}
	return true;
}
