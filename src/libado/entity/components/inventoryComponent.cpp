/*
 * inventoryComponent.cpp
 *
 *  Created on: 5 Aug 2017
 *      Author: linbob
 */

#include "inventoryComponent.hpp"
#include "../entityFactory.hpp"

InventoryComponent::InventoryComponent(EntityFactory* f, float max) : max(max), factory(f){
	inventory = std::vector<int>();
}
InventoryComponent::InventoryComponent(EntityFactory* f, rapidjson::Value& jsonData) : factory(f){
	max = jsonData["max"].GetFloat();
	inventory = std::vector<int>();
}
bool InventoryComponent::addItem(anax::Entity i){
	//removes entity from systems processing
	i.deactivate();

	if(i.hasComponent<WeightComponent>()){
		WeightComponent& w = i.getComponent<WeightComponent>();
		if(count + w.getWeight() > max) return false;

		if(i.hasComponent<StackableComponent>()){
			int s = i.getComponent<StackableComponent>().count;
			count += w.getWeight() * s;
		}else{
			count += w.getWeight();
		}
	}
	if(i.hasComponent<StackableComponent>()){
		std::string itemName = i.getComponent<NameComponent>().name;
		for(int& id : inventory){
			anax::Entity e = i.getWorld().getEntity(id);
			if(e.getComponent<NameComponent>().name.compare(itemName) == 0){
				e.getComponent<StackableComponent>().count +=
						i.getComponent<StackableComponent>().count;
				i.kill();
				return true;
			}
		}
	}

	inventory.push_back(i.getId().index);

	return true;
}
/*
 * if count == 0 : remove all of specified item
 * else if count >= stack.count : remove item as above; else remove weight of count * items then stack.count - count
 */
void InventoryComponent::removeItem(int id, int c){
	if(c == 0){
		anax::Entity e = factory->world->getEntity(id);
		if(e.hasComponent<WeightComponent>()){
			WeightComponent& w = e.getComponent<WeightComponent>();

			count -= w.getWeight();
		}
		inventory.erase(std::remove_if(inventory.begin(), inventory.end(),
				[&](int const& i) -> bool
				{
					return id == i;
				}));
	}else{
		for(int& i : inventory){
			if(i == id){
				anax::Entity e = factory->world->getEntity(i);
				StackableComponent& stack = e.getComponent<StackableComponent>();
				if(c >= stack.count){
					if(e.hasComponent<WeightComponent>()){
						WeightComponent& w = e.getComponent<WeightComponent>();

						count -= w.getWeight() * stack.count;
					}
					inventory.erase(std::remove_if(inventory.begin(), inventory.end(),
							[&](int const& i) -> bool
							{
								return id == i;
							}));
				}else{
					if(e.hasComponent<WeightComponent>()){
						WeightComponent& w = e.getComponent<WeightComponent>();

						count -= w.getWeight() * c;
					}
					stack.count -= c;
				}

				return;
			}
		}
	}
}
bool InventoryComponent::save(rapidjson::Value& entData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
	rapidjson::Value inv(rapidjson::kArrayType);

	for(int id : inventory) {
		anax::Entity item = factory->world->getEntity(id);
		factory->saveEntity(item, inv, dAlloc);
	}

	entData.AddMember("inv", inv, dAlloc);
	return true;
}
bool InventoryComponent::load(rapidjson::Value& jsonData){
	for (auto& i : jsonData.GetArray()){
		std::string name = i["name"].GetString();
		anax::Entity e = factory->createEntity(name);

		factory->loadEntity(e, i);

		addItem(e);
	}
	return true;
}
