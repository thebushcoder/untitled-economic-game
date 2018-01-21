/*
 * inventory.hpp
 *
 *  Created on: 28 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_INVENTORYCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_INVENTORYCOMPONENT_HPP_

#include <rapidjson/document.h>
#include <anax/Component.hpp>
#include <anax/World.hpp>
#include <sstream>

#include "serialComponent.hpp"
#include "weightComponent.hpp"
#include "stackableComponent.hpp"
#include "nameComponent.hpp"

class EntityFactory;

//Inventory item save + load is handled in Entity factory when saving/loading parent entity
struct InventoryComponent : anax::Component, public SerializableComponent{
	std::vector<int> inventory;
	float max, count = 0;

	InventoryComponent(EntityFactory* factory, float max = 10);
	InventoryComponent(EntityFactory* factory, rapidjson::Value& jsonData);
	bool addItem(anax::Entity i);
	/*
	 * if count == 0 : remove all of specified item
	 * else if count >= stack.count : remove item as above; else remove weight of count * items then stack.count - count
	 */
	void removeItem(int id, int c = 0);

	bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);
	bool load(rapidjson::Value& jsonData);
private:
	EntityFactory* factory = nullptr;
};

#endif /* LIBADO_ENTITY_COMPONENTS_INVENTORYCOMPONENT_HPP_ */
