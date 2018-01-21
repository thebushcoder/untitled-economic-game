/*
 * entityFactory.cpp
 *
 *  Created on: 27 Feb 2016
 *      Author: linbob
 */

#include "entityFactory.hpp"

/*
 * STORES THE JSON PROFILE COLLECTION AND REFERENCEs IT DURING ENTITY CREATION
 */
EntityFactory::EntityFactory(std::string jsonPath, anax::World* world,
		TextureAtlas* atlas) : world(world), atlas(atlas){
	loadEntityProfiles(jsonPath);
}
//used for read only operations. DO NOT ADD TO JSON VALUE
rapidjson::Value& EntityFactory::getRawEntityData(std::string name){
	return entityProfiles[name.c_str()];
}

void EntityFactory::loadEntityProfiles(std::string jsonPath){
	char readBuffer[65536];

	FILE* fp = fopen(jsonPath.c_str(), "r"); // non-Windows use "r"
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	entityProfiles.ParseStream(is);

	fclose(fp);
}

anax::Entity EntityFactory::createEntity(std::string name){
	anax::Entity e = world->createEntity();

	e.addComponent<PositionComponent>();

	e.addComponent<NameComponent>(name);

	rapidjson::Value& profile = entityProfiles[name.c_str()];
	//Must be non-const member iterator so that refs can be passed
	for (rapidjson::Value::MemberIterator itr = profile.MemberBegin();
	    itr != profile.MemberEnd(); ++itr)	{

		if(strcmp(itr->name.GetString(), "sprite") == 0){
			e.addComponent<SpriteComponent>(itr->value, *atlas);
		}
		if(strcmp(itr->name.GetString(), "player") == 0){
			e.addComponent<PlayerComponent>();
			player = e;
		}
		if(strcmp(itr->name.GetString(), "tile_data") == 0){
			e.addComponent<TileDataComponent>(itr->value.GetObject()["cost"].GetInt(), itr->value.GetObject()["priority"].GetInt());
		}
		if(strcmp(itr->name.GetString(), "ai") == 0){
			e.addComponent<AiComponent>(itr->value.GetString());
		}
		if(strcmp(itr->name.GetString(), "item") == 0){
			e.addComponent<ItemComponent>(itr->value["type"]);
			if(itr->value.HasMember("stackable") && !e.hasComponent<StackableComponent>()){
				e.addComponent<StackableComponent>();
			}
		}
		if(strcmp(itr->name.GetString(), "inventory") == 0){
			e.addComponent<InventoryComponent>(this, itr->value);
		}
		if(strcmp(itr->name.GetString(), "shop") == 0){
			e.addComponent<ShopComponent>();
		}
		if(strcmp(itr->name.GetString(), "money") == 0){
			e.addComponent<MoneyComponent>();
		}
		if(strcmp(itr->name.GetString(), "value") == 0){
			e.addComponent<ValueComponent>(itr->value.GetInt());
		}
		if(strcmp(itr->name.GetString(), "attack") == 0){
			e.addComponent<AttackComponent>(itr->value.GetInt());
		}
		if(strcmp(itr->name.GetString(), "health") == 0){
			e.addComponent<HealthComponent>(itr->value.GetInt());
		}
		if(strcmp(itr->name.GetString(), "weight") == 0){
			e.addComponent<WeightComponent>(itr->value.GetFloat());
		}
		if(strcmp(itr->name.GetString(), "energy") == 0){
			e.addComponent<EnergyComponent>(itr->value.GetInt());
		}
		if(strcmp(itr->name.GetString(), "description") == 0){
			e.addComponent<DescriptionComponent>(itr->value.GetString());
		}
		if(strcmp(itr->name.GetString(), "lootlist") == 0){
			e.addComponent<LootListComponent>(itr->value);
		}
		if(strcmp(itr->name.GetString(), "prod") == 0){
			//check if entity already has resources component and loads production resource entities into res
			if(!e.hasComponent<ResourcesComponent>()){
				e.addComponent<ResourcesComponent>();
			}
			ResourcesComponent& res = e.getComponent<ResourcesComponent>();
			ProductionComponent& prod = e.addComponent<ProductionComponent>();

			//add production values to production component
			for(auto& r : itr->value.GetArray()){
				std::string n = r["name"].GetString();
				anax::Entity resEnt = createEntity(n);
				//remove unnecessary components
				if(resEnt.hasComponent<PositionComponent>()){
					resEnt.removeComponent<PositionComponent>();
				}
				if(resEnt.hasComponent<TileDataComponent>()){
					resEnt.removeComponent<TileDataComponent>();
				}
				//ensure resource has a stackable component
				if(!resEnt.hasComponent<StackableComponent>()){
					resEnt.addComponent<StackableComponent>();
				}
				//add res ent to res component
				//res doesnt need system processing
				resEnt.deactivate();
				ResourcesComponent::Resource resource;
				resource.e = resEnt;
				res.map[n] = resource;

				//load production data - res name, production qty and % workers assigned
				prod.producedResources[r["name"].GetString()] =
						std::pair<std::pair<float,float>, float>(
								std::pair<float,float>(r["qty"].GetFloat(), 0),
								r["workDist"].GetFloat()
						);
			}
		}
		if(strcmp(itr->name.GetString(), "cons") == 0){
			e.addComponent<ConsumptionComponent>(itr->value);
		}
		if(strcmp(itr->name.GetString(), "res") == 0 && !e.hasComponent<ResourcesComponent>()){
			e.addComponent<ResourcesComponent>();
		}
		if(strcmp(itr->name.GetString(), "portal") == 0){
			e.addComponent<PortalComponent>(itr->value.GetString());
		}
		if(strcmp(itr->name.GetString(), "pop") == 0){
			e.addComponent<PopulationComponent>(itr->value.GetInt());
		}
		if(strcmp(itr->name.GetString(), "slots") == 0){
			e.addComponent<EquipmentComponent>(this, itr->value);
		}
		if(strcmp(itr->name.GetString(), "mSpeed") == 0){
			e.addComponent<MoveSpeedComponent>(itr->value.GetFloat());
		}
	}

	if(e.hasComponent<PlayerComponent>()){
		AiComponent& ai = e.getComponent<AiComponent>();
		ai.board->SetBool("needsInput", true);
	}

	e.activate();

	return e;
}
void EntityFactory::saveEntity(anax::Entity& e, rapidjson::Value& entData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){

	//if entity is a mount and is mounted, continue
	if(e.hasComponent<OwnerComponent>()) return;

	rapidjson::Value t(rapidjson::kObjectType);

	for(anax::Component* c : e.getComponents()){
		//entity component vectors are initialized with 64 null pointers > hence null check below
		if(!c) continue;

		if(SerializableComponent* s = dynamic_cast<SerializableComponent*>(c)){
			s->save(t, dAlloc);
		}
	}

	if(entData.IsArray()){
		entData.PushBack(t, dAlloc);
	}else{
		entData.AddMember("entity", t, dAlloc);
	}
}
void EntityFactory::loadEntity(anax::Entity& e, rapidjson::Value& jsonData){
	for (rapidjson::Value::MemberIterator itr = jsonData.MemberBegin();
		itr != jsonData.MemberEnd(); ++itr)	{

		if(strcmp(itr->name.GetString(), "ai") == 0){
			AiComponent& ai = e.getComponent<AiComponent>();
			ai.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "item") == 0){
			ItemComponent& item = e.getComponent<ItemComponent>();
			item.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "pos") == 0){
			PositionComponent& p = e.getComponent<PositionComponent>();
			p.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "hp") == 0){
			HealthComponent& hp = e.getComponent<HealthComponent>();
			hp.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "inv") == 0){
			InventoryComponent& inven = e.getComponent<InventoryComponent>();
			inven.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "stackable") == 0){
			StackableComponent& stack = e.getComponent<StackableComponent>();
			stack.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "money") == 0){
			MoneyComponent& money = e.getComponent<MoneyComponent>();
			money.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "res") == 0){
			ResourcesComponent& res = e.getComponent<ResourcesComponent>();
			res.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "equip") == 0){
			EquipmentComponent& eq = e.getComponent<EquipmentComponent>();
			eq.load(itr->value);
		}
		if(strcmp(itr->name.GetString(), "mount") == 0){
			std::string name = itr->value["name"].GetString();
			anax::Entity m = createEntity(name);
			loadEntity(m, itr->value);
			e.addComponent<MountComponent>(this, m);
		}
		if(strcmp(itr->name.GetString(), "energy") == 0){
			EnergyComponent& energy = e.getComponent<EnergyComponent>();
			energy.load(itr->value);
		}
	}
}
anax::Entity& EntityFactory::getPlayer(){
	return player;
}
