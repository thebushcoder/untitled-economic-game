/*
 * equipmentComponent.hpp
 *
 *  Created on: 30 Mar 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_EQUIPMENTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_EQUIPMENTCOMPONENT_HPP_

#include <memory>

#include <anax/Component.hpp>
#include <TGUI/TGUI.hpp>

#include "../../gui/slotWidget.hpp"
#include "serialComponent.hpp"

class EntityFactory;
///////////////////////////////////////////////////////////////
struct EquipmentSlot{
	int entityId;
	Item::Type type;
	int id;

	EquipmentSlot(Item::Type t, int i = 1) : entityId(-1), type(t), id(i){}

	bool equipItem(anax::Entity i){
		if(entityId != -1){
			printf("Error: equipment slot is already occupied!\n");
			//FUTURE: swap items.
			//move currently equip to inven(or ground if inven is full)
			return false;
		}

		entityId = i.getId().index;

		return true;
	}
	void removeItem(){
		entityId = -1;
	}
	bool hasItem(){
		return entityId != -1;
	}
	int getSlotItem(){
		return entityId;
	}
	Item::Type getSlotType(){
		return type;
	}
	int getId(){
		return id;
	}

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
		return false;}
	bool load(rapidjson::Value& jsonData){return false;}
};
///////////////////////////////////////////////////////////////

struct EquipmentComponent : anax::Component, SerializableComponent{
	EquipmentComponent(EntityFactory* factory, rapidjson::Value& jsonData);

	EquipmentSlot* getSlot(Item::Type type, int slotId = 1);

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);
	bool load(rapidjson::Value& jsonData);

protected:
	std::vector<std::shared_ptr<EquipmentSlot>> list;
	EntityFactory* factory = nullptr;
};



#endif /* LIBADO_ENTITY_COMPONENTS_EQUIPMENTCOMPONENT_HPP_ */
