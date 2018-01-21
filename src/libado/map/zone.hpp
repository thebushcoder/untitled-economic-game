/*
 * zone.hpp
 *
 *  Created on: 1 Dec 2016
 *      Author: linbob
 */

#ifndef LIBADO_MAP_ZONE_HPP_
#define LIBADO_MAP_ZONE_HPP_

#include <regex>

#include "tileMap.hpp"
#include "../worldView.hpp"
#include "../entity/entityFactory.hpp"

struct Zone{
	std::unique_ptr<TileMap> zone;
	std::unique_ptr<WorldView> zoneView;

	bool active = false, loaded = false;

	Zone(int windowW, int windowH, std::string inputExt, ThorInput& input);
	virtual ~Zone(){}

	void initTileMap(TileMap* map);
	virtual void saveZone(std::string filePath = "");
	virtual void loadZone(std::string filePath, EntityFactory* entFactory);
	virtual void unloadZone(std::string filePath = "");
	virtual void render(sf::RenderWindow* window);
	virtual void resize(int w, int h);

	bool isActive(){
		return (active && loaded);
	}
	bool isLoaded(){
		return loaded;
	}
	void setActive(bool flag){
		active = flag;
	}
	void setParent(Zone* p){
		parent = p;
	}
	Zone* getParent(){
		return parent;
	}
	std::string getFilePath(){
		return zoneFilePath;
	}
	void setPath(std::string path){
		zoneFilePath = path;
	}
protected:
	std::string zoneFilePath;
	Zone* parent = nullptr;
};

struct Overworld : public Zone{
	std::unique_ptr<Zone> child;

	Overworld(int windowW, int windowH, std::string inputExt, ThorInput& input);
	void initTileMap(TileMap* map);
	void render(sf::RenderWindow* window);
	void resize(int w, int h);

	virtual void saveZone(std::string filePath = "");
	void saveOverworld(rapidjson::Document& json);
	void loadOverworld(rapidjson::Document& json, EntityFactory* entFactory);
	void loadChildZone(std::string filePath, EntityFactory* entFactory);
	void unloadChildZone(std::string filePath = "");
	void setChildActive(bool flag = true);
	bool isChildActive();
	Zone* getActiveZone();
};


#endif /* LIBADO_MAP_ZONE_HPP_ */
