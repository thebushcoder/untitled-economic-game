/*
 * entityLayer.cpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#include "entityLayer.hpp"

#include "tileMap.hpp"

EntityLayer::EntityLayer(EntityFactory* factory, TileMap* tileMap, int width, int height) :
	factory(factory), tileMap(tileMap){
	map.resize(width);
	for (int i = 0; i < width; ++i){
		map[i].resize(height);
	}
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			map[x][y] = std::vector<EntityRefNode>();
		}
	}
}
void EntityLayer::resize(int w, int h){
	int wDiff = w - map.size();
	int hDiff = h - map[0].size();

	map.resize(w);
	for (int i = 0; i < w; ++i){
		map[i].resize(h);
	}

	for(int x = map.size() - wDiff; x < map.size(); ++x){
		for(int y = map[0].size() - hDiff; y < map[x].size(); ++y){
			map[x][y] = std::vector<EntityRefNode>();
		}
	}
}
void EntityLayer::load(rapidjson::Value& layerData){
	for(int j = 0; j < layerData.Size(); ++j){
		std::string name = layerData[j]["name"].GetString();

		anax::Entity e = factory->createEntity(name);

		factory->loadEntity(e, layerData[j]);

		if(e.hasComponent<PortalComponent>()){
			portalList[(e.getComponent<PortalComponent>()).destZoneName] = e.getId().index;
		}

		PositionComponent& p = e.getComponent<PositionComponent>();
		setEntity(p.tilePosition.x, p.tilePosition.y, e);
	}
}
void EntityLayer::save(rapidjson::Value& layerData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			if(map[x][y].empty()) continue;
			for(EntityRefNode node : map[x][y]){
				anax::Entity e = factory->world->getEntity(node.id);

				factory->saveEntity(e, layerData, dAlloc);
			}
		}
	}
}
void EntityLayer::render(sf::RenderWindow* window){
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			if(map[x][y].empty()) continue;

			for(EntityRefNode& n : map[x][y]){
				int i = n.id;
				anax::Entity entity = factory->world->getEntity(i);
				PositionComponent& p = entity.getComponent<PositionComponent>();
				if(entity.hasComponent<MountComponent>()){
					MountComponent& m = entity.getComponent<MountComponent>();
					if(!m.getMount().isActivated()){
						SpriteComponent& mSprite = m.getMount().getComponent<SpriteComponent>();

						mSprite.sprite->setPosition(p.screenPosition.x, p.screenPosition.y);

						window->draw(*mSprite.sprite);
					}
				}

				SpriteComponent& s = entity.getComponent<SpriteComponent>();

				s.sprite->setPosition(p.screenPosition.x, p.screenPosition.y);

				window->draw(*s.sprite);
			}
		}
	}
}
void EntityLayer::removeEntity(int x, int y, int id){
	map[x][y].erase(std::remove_if(map[x][y].begin(), map[x][y].end(),
	[&](EntityRefNode const& n) -> bool
	{
		return n.id == id;
	}));
}
std::vector<EntityRefNode>& EntityLayer::getEntitiesAt(int x, int y){
	return map[x][y];
}
bool EntityLayer::isOccupied(int x, int y){
	return !map[x][y].empty();
}
anax::Entity EntityLayer::createLootDrop(){
	anax::Entity e = factory->world->createEntity();

	e.addComponent<NameComponent>("Loot drop");
	e.addComponent<PositionComponent>();
	e.addComponent<SpriteComponent>("items/087", *factory->atlas);
	e.addComponent<InventoryComponent>(factory, 32);

	return e;
}
void EntityLayer::setEntity(int x, int y, anax::Entity entity){
	if(tileMap->getName().compare("overworld.save") == 0){
		entity.removeComponent<ZoneComponent>();
	}else{
		if(entity.hasComponent<ZoneComponent>()){
			entity.getComponent<ZoneComponent>().zoneName = tileMap->getName();
		}else{
			entity.addComponent<ZoneComponent>(tileMap->getName());
		}
	}

	if(entity.hasComponent<ItemComponent>() && !entity.hasComponent<AiComponent>()){
		for(EntityRefNode& ref : map[x][y]){
			anax::Entity e = factory->world->getEntity(ref.id);
			std::string refName = e.getComponent<NameComponent>().name;
			if(refName.compare("Loot drop") == 0){
				InventoryComponent& inven = e.getComponent<InventoryComponent>();

				inven.addItem(entity);
				return;
			}
		}

		anax::Entity drop = createLootDrop();
		PositionComponent& p = drop.getComponent<PositionComponent>();
		p.setPosition(x, y, x * TILESIZE, y * TILESIZE);
		InventoryComponent& inven = drop.getComponent<InventoryComponent>();
		inven.addItem(entity);

		map[x][y].push_back(EntityRefNode(drop.getId().index));
		return;
	}
	if(entity.hasComponent<TileDataComponent>()){
		TileDataComponent& tc = entity.getComponent<TileDataComponent>();
		map[x][y].push_back(EntityRefNode(entity.getId().index, tc.cost, tc.priority));
	}else{
		map[x][y].push_back(EntityRefNode(entity.getId().index));
	}
	std::sort(map[x][y].begin(), map[x][y].end());
}
bool EntityLayer::hasPortal(std::string zoneName){
	return portalList.find(zoneName) != portalList.end();
}
int EntityLayer::getPortalId(std::string zoneName){
	return portalList[zoneName];
}
int EntityLayer::getWidth(){
	return map.size();
}
int EntityLayer::getHeight(){
	return map[0].size();
}
