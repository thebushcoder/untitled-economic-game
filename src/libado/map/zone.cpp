/*
 * zone.cpp
 *
 *  Created on: 1 Dec 2016
 *      Author: linbob
 */

#include "zone.hpp"

Zone::Zone(int windowW, int windowH, std::string inputExt, ThorInput& input) :
	zoneView(std::unique_ptr<WorldView>(new WorldView(windowW, windowH, inputExt, input, this))){
}
void Zone::initTileMap(TileMap* map){
	zone = std::unique_ptr<TileMap>(std::move(map));
}
void Zone::loadZone(std::string filePath, EntityFactory* entFactory){
	//save filePath for unloading zone during gameplay
	zoneFilePath = filePath;

	zone->load(filePath, entFactory);
	loaded = true;
}
void Zone::saveZone(std::string filePath){
	if(filePath.empty()){
		zone->save(zoneFilePath);
	}else{
		zone->save(filePath);
	}
	if(getParent() && isActive()){
		getParent()->saveZone();
	}
}
void Zone::unloadZone(std::string filePath){
	//tilemap unload + delete
	saveZone(filePath);
	//kill entities
	EntityLayer& layer = zone->getEntityLayer();
	anax::World* world = layer.getFactory()->world;
	for(int x = 0; x < layer.getWidth(); ++x){
		for(int y = 0; y < layer.getHeight(); ++y){
			for(auto& entRef : layer.getEntitiesAt(x, y)){
				world->getEntity(entRef.id).kill();
			}
		}
	}

	loaded = false;
	active = false;
}
void Zone::render(sf::RenderWindow* window){
	window->setView(*zoneView->view);

	zone->render(window);
}
void Zone::resize(int w, int h){
	zoneView->resize(w, h);
}
Overworld::Overworld(int windowW, int windowH, std::string inputExt, ThorInput& input) :
	Zone(windowW, windowH, inputExt, input){
	child = std::unique_ptr<Zone>(new Zone(windowW, windowH, "childZone", input));
}
void Overworld::render(sf::RenderWindow* window){
	if(child->isActive()){
		child->render(window);
	}else{
		Zone::render(window);
	}
}
void Overworld::initTileMap(TileMap* map){
	Zone::initTileMap(map);
	child->initTileMap(new TileMap(zone->getEntityLayer().getFactory(),
			zone->getFactory(), 32, 32));
}
void Overworld::loadChildZone(std::string filePath, EntityFactory* entFactory){
	child->loadZone(filePath, entFactory);

	if(!child->getParent()){
		child->setParent(this);
	}
}
void Overworld::saveZone(std::string filePath){
	if(filePath.empty()){
		zone->save(zoneFilePath);
	}else{
		zone->save(filePath);
	}
	if(child && isActive()){
		child->saveZone();
	}
}
void Overworld::saveOverworld(rapidjson::Document& json){
	try{
		zone->save(json);

		if(child->isLoaded()){
			child->saveZone();
		}
	}catch(std::exception& e){
		printf("Save Zone Error : %s\n", e.what());
	}
}
void Overworld::loadOverworld(rapidjson::Document& json, EntityFactory* entFactory){
	zone->load(json, entFactory);
	loaded = true;
}
void Overworld::unloadChildZone(std::string filePath){
	if(child){
		child->unloadZone(filePath);
	}
}
void Overworld::resize(int w, int h){
	Zone::resize(w, h);

	if(child){
		child->resize(w, h);
	}
}
void Overworld::setChildActive(bool flag){
	if(flag){
		this->active = false;
		child->setActive(true);
	}else{
		this->active = true;
		child->setActive(false);
	}
}
bool Overworld::isChildActive(){
	return child->isActive();
}
Zone* Overworld::getActiveZone(){
	if(isChildActive()){
		return child.get();
	}else{
		return this;
	}
}
