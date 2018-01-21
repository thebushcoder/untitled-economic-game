/*
 * LootListManager.hpp
 *
 *  Created on: 19 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_LOOTLISTMANAGER_HPP_
#define LIBADO_ENTITY_LOOTLISTMANAGER_HPP_

#include <random>
#include <chrono>

#include "entityFactory.hpp"
#include "components/itemTypes.hpp"
#include "lootList.hpp"

struct LootListManager{
protected:
	std::mt19937 gen;

	//Item name
	struct Entry{
		std::string name;
		int requiredLevel = 0;	//minimum required level to use item

		Entry(std::string n) : name(n){}
		std::string getName(){
			return name;
		}
		virtual bool isCatagory(){
			return false;
		}
	};
	//Item Type catagory : weapons, wearable, consumable
	struct CatagoryEntry : Entry{
		CatagoryEntry(std::string n, Item::Type t) : Entry(n), type(t){
			children = std::vector<std::shared_ptr<Entry>>();
		}
		virtual bool isCatagory(){
			return true;
		}
		Item::Type getType(){
			return type;
		}
		Entry* findChild(std::string name){
			for(std::shared_ptr<Entry>& e : children){
				CatagoryEntry* c = dynamic_cast<CatagoryEntry*>(e.get());
				if(name.compare(e->getName()) == 0){
					return e.get();
				}else if(c != nullptr){
					Entry* f = c->findChild(name);
					if(f != nullptr){
						return f;
					}
				}
			}
			return nullptr;
		}
		void add(std::shared_ptr<Entry> e){
			children.push_back(e);
		}
		//get children including catagories(excluding their children)
		std::vector<std::shared_ptr<Entry>>& getAllChildren(){
			return children;
		}
		//get all leaf(item) entries. searches child catagories for leaf entries but
		//doesnt return CatagoryEntries
		std::vector<Entry*> getChildren(){
			std::vector<Entry*> list;

			for(std::shared_ptr<Entry>& e : children){
				CatagoryEntry* c = dynamic_cast<CatagoryEntry*>(e.get());
				if(c != nullptr){
					std::vector<Entry*> r = c->getChildren();
					for(Entry* l : r){
						CatagoryEntry* leaf = dynamic_cast<CatagoryEntry*>(l);
						if(!leaf){
							list.push_back(l);
						}
					}
				}else{
					list.push_back(e.get());
				}
			}

			return list;
		}
	protected:
		Item::Type type;
		std::vector<std::shared_ptr<Entry>> children;
	};

	std::vector<std::shared_ptr<Entry>> catagories;

private:
	EntityFactory* factory;

	void loadItemData(rapidjson::Value& jsonData);
	void createCatagories();
	CatagoryEntry* addItem(Item::Type t, std::vector<std::shared_ptr<Entry>>& list);
	Entry* getRandomItem(LootListEntry& e);
	void generateItemCount(anax::Entity& e, std::vector<anax::Entity>& returnList,
			std::string name,int count);
	void addItemToList(std::vector<anax::Entity>& returnList,
			std::string name, int count);
public:
	LootListManager(EntityFactory* factory);

	CatagoryEntry* getEntry(std::string name);
	std::vector<anax::Entity> generateLoot(LootList& list);
};


#endif /* LIBADO_ENTITY_LOOTLISTMANAGER_HPP_ */
