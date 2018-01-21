/*
 * lootListManager.cpp
 *
 *  Created on: 19 Oct 2016
 *      Author: linbob
 */

#include "lootListManager.hpp"

LootListManager::LootListManager(EntityFactory* factory) : factory(factory){
	gen = std::mt19937(std::chrono::system_clock::now().
			time_since_epoch() / std::chrono::milliseconds(1));

	catagories = std::vector<std::shared_ptr<Entry>>();

	createCatagories();

	loadItemData(factory->entityProfiles);
}
//only used to look up catagories, NOT raw items
LootListManager::CatagoryEntry* LootListManager::getEntry(std::string name){
	std::string rawName = name.substr(1, name.size() - 2);

	for(std::shared_ptr<Entry>& e : catagories){
		CatagoryEntry* c = dynamic_cast<CatagoryEntry*>(e.get());

		if(c->getName().compare(rawName) == 0){
			return c;
		}
		Entry* child = c->findChild(rawName);
		if(child != nullptr){
			return dynamic_cast<CatagoryEntry*>(child);
		}
	}
	return nullptr;
}
std::vector<anax::Entity> LootListManager::generateLoot(LootList& list){
	std::vector<anax::Entity> returnList;

	std::uniform_real_distribution<> dist(0.0f, 1.0f);

	//iterate loot list
	for(LootListEntry& e : list.entries){
		float roll = dist(gen);
		//check if item will drop
		if(roll <= e.dropChance){
			int countRoll = 1;
			if(e.maxCount > 1){
				std::uniform_real_distribution<> countDis(e.minCount, e.maxCount);
				//roll item drop count
				countRoll = countDis(gen);
			}

			if(e.isCatagory()){
				Entry* item;
				//get an item of catagory type that is <= minLevel
				do{
					item = getRandomItem(e);
				}while(item->requiredLevel < e.minLevel || item->requiredLevel > e.maxLevel);

				addItemToList(returnList, item->name, countRoll);
			}else{
				addItemToList(returnList, e.name, countRoll);
			}
		}
	}

	return returnList;
}
void LootListManager::addItemToList(std::vector<anax::Entity>& returnList,
		std::string name, int count){
	if(count > 1){
		anax::Entity entity = factory->createEntity(name);
		returnList.push_back(entity);
		generateItemCount(entity, returnList, name, count);
	}else{
		returnList.push_back(factory->createEntity(name));
	}
}
void LootListManager::generateItemCount(anax::Entity& e, std::vector<anax::Entity>& returnList,
		std::string name, int count){
	for(int i = 1; i < count; ++i){
		if(e.hasComponent<StackableComponent>()){
			StackableComponent& s = e.getComponent<StackableComponent>();
			++s.count;
		}else{
			returnList.push_back(factory->createEntity(name));
		}
	}
}
LootListManager::Entry* LootListManager::getRandomItem(LootListEntry& e){
	CatagoryEntry* c = getEntry(e.name);
	std::vector<Entry*> children = c->getChildren();

	//select random item in catagory
	std::uniform_real_distribution<> sizeDis(0, children.size() - 1);
	int itemIndex = sizeDis(gen);

	return children[itemIndex];
}
void LootListManager::loadItemData(rapidjson::Value& jsonData){
	for (rapidjson::Value::MemberIterator i = jsonData.MemberBegin();
		i != jsonData.MemberEnd(); ++i){
		if(i->value.HasMember("item")){
			rapidjson::Value& v = i->value["item"]["type"];

			std::vector<Item::Type> usedList = std::vector<Item::Type>();

			std::shared_ptr<Entry> e = std::shared_ptr<Entry>(
					new Entry(i->name.GetString())
			);

			if(i->value.HasMember("requiredLevel")){
				e->requiredLevel = i->value["requiredLevel"].GetInt();
			}

			for(auto& j : v.GetArray()){
				CatagoryEntry* c = addItem((Item::Type)j.GetInt(), catagories); //nullptr
				if(c != nullptr){
					//check if item's other types are children of catagory
					if(v.GetArray().Size() > 1){
						std::vector<std::shared_ptr<Entry>>& children = c->getAllChildren();
						for(auto& t : v.GetArray()){
							for(std::shared_ptr<Entry>& e : children){
								CatagoryEntry* childCat = dynamic_cast<CatagoryEntry*>(e.get());
								if(childCat){
									if(childCat->getType() == (Item::Type)t.GetInt()){
										c = childCat;
									}
								}
							}
						}
					}
					//add item entry to catagory
					c->add(e);
				}
			}
		}
	}
}
LootListManager::CatagoryEntry* LootListManager::addItem(Item::Type t,
		std::vector<std::shared_ptr<Entry>>& list){

	//search list - eg catagories list
	for(std::shared_ptr<Entry>& e : list){
		CatagoryEntry* c = dynamic_cast<CatagoryEntry*>(e.get());
		if(!c) continue;

		//if catagory entry is = current type
		if(c->getType() == t){
			return c;
		}else{
			CatagoryEntry* childCat = addItem(t, c->getAllChildren());
			if(childCat){
				return childCat;
			}
		}
	}
	return nullptr;
}
void LootListManager::createCatagories(){
	//WEAPONS
	std::shared_ptr<Entry> c = std::shared_ptr<Entry>(
			new CatagoryEntry("weapon", Item::Type::WEAPON)
	);
	std::shared_ptr<Entry> e = std::shared_ptr<Entry>(
			new CatagoryEntry("melee", Item::Type::MELEE)
	);
	((CatagoryEntry&)*c).add(e);
	e = std::shared_ptr<Entry>(
			new CatagoryEntry("ranged", Item::Type::RANGED)
	);
	((CatagoryEntry&)*c).add(e);

	catagories.push_back(c);

	//WEARABLES
	c = std::shared_ptr<Entry>(
			new CatagoryEntry("wearable", Item::Type::WEARABLE)
	);
	catagories.push_back(c);

	//CONSUMABLES
	c = std::shared_ptr<Entry>(
			new CatagoryEntry("consumable", Item::Type::CONSUMABLE)
	);
	e = std::shared_ptr<Entry>(
			new CatagoryEntry("food", Item::Type::FOOD)
	);
	((CatagoryEntry&)*c).add(e);
	e = std::shared_ptr<Entry>(
			new CatagoryEntry("ingredient", Item::Type::RAW_GOOD)
	);
	((CatagoryEntry&)*c).add(e);
	e = std::shared_ptr<Entry>(
			new CatagoryEntry("drug", Item::Type::DRUG)
	);
	((CatagoryEntry&)*c).add(e);
	catagories.push_back(c);

	//COMMODITIES
	c = std::unique_ptr<Entry>(
			new CatagoryEntry("commodity", Item::Type::COMMODITY)
	);
	e = std::shared_ptr<Entry>(
			new CatagoryEntry("drug", Item::Type::DRUG)
	);
	((CatagoryEntry&)*c).add(e);
	catagories.push_back(c);

	//TOOL
	c = std::unique_ptr<Entry>(
			new CatagoryEntry("tool", Item::Type::TOOL)
	);
	catagories.push_back(c);

	//MISC
	c = std::unique_ptr<Entry>(
			new CatagoryEntry("misc", Item::Type::MISC)
	);
	catagories.push_back(c);
}
