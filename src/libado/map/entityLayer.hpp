/*
 * entityLayer.hpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#ifndef LIBADO_MAP_ENTITYLAYER_HPP_
#define LIBADO_MAP_ENTITYLAYER_HPP_

#include <vector>
#include <memory>

#include <SFML/Window.hpp>
#include <anax/Entity.hpp>
#include <anax/World.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "layer.hpp"
#include "../entity/entityFactory.hpp"
//#include "../map/zone.hpp"

struct EntityRefNode{
	int id;
	int cost;
	int priority;	//render priority(decending order)

	EntityRefNode(int id, int cost = 0, int priority = 0) : id(id), cost(cost), priority(priority){}

	bool operator < (const EntityRefNode& other) const{
		return this->priority < other.priority;
	}
};

class TileMap;

class EntityLayer : Layer{
public:
	EntityLayer(){}
	EntityLayer(EntityFactory* factory, TileMap* tileMap, int width, int height);

	void load(rapidjson::Value& layerData);
	void save(rapidjson::Value& layerData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);
	virtual void render(sf::RenderWindow* window);
	virtual void resize(int w, int h);
	virtual int getWidth();
	virtual int getHeight();
	virtual bool isOccupied(int x, int y);
	std::vector<EntityRefNode>& getEntitiesAt(int x, int y);
	void setEntity(int x, int y, anax::Entity entity);
	void removeEntity(int x, int y, int id);
	bool hasPortal(std::string zoneName);
	int getPortalId(std::string zoneName);
	EntityFactory* getFactory(){
		return factory;
	}
	anax::Entity createLootDrop();
protected:
	EntityFactory* factory;
	TileMap* tileMap;

	std::vector<std::vector<std::vector<EntityRefNode>> > map;
	std::map<std::string, int> portalList; // destZoneName, entityId
};

#endif /* LIBADO_MAP_ENTITYLAYER_HPP_ */
