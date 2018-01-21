/*
 * tileMap.cpp
 *
 *  Created on: 9 Feb 2016
 *      Author: linbob
 */

#include "tileMap.hpp"

TileMap::TileMap(EntityFactory* factory, std::shared_ptr<TileFactory> tileFactory, int w, int h) :
	width(w), height(h){
	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));
	entityLayer = std::unique_ptr<EntityLayer>(new EntityLayer(factory, this, width, height));
	TileMap::factory = tileFactory;
}
TileMap::TileMap(EntityFactory* factory, std::string jsonPath, int w, int h,
		std::shared_ptr<TextureAtlas>& atlas) : width(w), height(h){
	TileMap::factory = std::shared_ptr<TileFactory>(new TileFactory(jsonPath, atlas));

	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));
	entityLayer = std::unique_ptr<EntityLayer>(new EntityLayer(factory, this, width, height));
}
void TileMap::render(sf::RenderWindow* window){
	tileLayer->render(window);
	entityLayer->render(window);
}
void TileMap::resize(int w, int h){
	width = w;
	height = h;

	tileLayer->resize(w, h);
	entityLayer->resize(w, h);
}
void TileMap::load(std::string filePath, EntityFactory* entFactory){
	mapName = filePath.substr(filePath.find_last_of("/") + 1);
	//READ MAP JSON
	char readBuffer[65536];
	rapidjson::Document d;

	FILE* fp = fopen(&filePath[0], "r");
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);

	fclose(fp);

	//LOAD MAP SIZE
	width = d["width"].GetInt();
	height = d["height"].GetInt();

	//reinitialize layers with saved width/height
	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));
	entityLayer = std::unique_ptr<EntityLayer>(new EntityLayer(entFactory, this, width, height));

	tileLayer->load(d["layers"]["tiles"], *factory);
	entityLayer->load(d["layers"]["entities"]);
}
void TileMap::save(std::string filePath){
	//CREATE JSON
	rapidjson::Document d;
	d.SetObject();

	//JSON-WRITE MAP SIZE
	d.AddMember("width", width, d.GetAllocator());
	d.AddMember("height", height, d.GetAllocator());

	//JSON-WRITE LAYERS + TILES
	rapidjson::Value layersJSON(rapidjson::kObjectType);

	rapidjson::Value layer(rapidjson::kArrayType);

	tileLayer->save(layer, d.GetAllocator());
	layersJSON.AddMember("tiles", layer, d.GetAllocator());

	rapidjson::Value entJSON(rapidjson::kArrayType);

	entityLayer->save(entJSON, d.GetAllocator());
	layersJSON.AddMember("entities", entJSON, d.GetAllocator());

	d.AddMember("layers", layersJSON, d.GetAllocator());

	//WRITE JSON TO FILE
	FILE* fp = fopen(filePath.c_str(), "w");

	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	d.Accept(writer);

	fclose(fp);
}
void TileMap::load(rapidjson::Document& d, EntityFactory* entFactory){
	//LOAD MAP SIZE
	width = d["width"].GetInt();
	height = d["height"].GetInt();

	//reinitialize layers with saved width/height
	tileLayer = std::unique_ptr<TileLayer>(new TileLayer(width, height));
	entityLayer = std::unique_ptr<EntityLayer>(new EntityLayer(entFactory, this, width, height));

	tileLayer->load(d["layers"]["tiles"], *factory);
	entityLayer->load(d["layers"]["entities"]);
}
void TileMap::save(rapidjson::Document& d){
	//JSON-WRITE MAP SIZE
	d.AddMember("width", width, d.GetAllocator());
	d.AddMember("height", height, d.GetAllocator());

	//JSON-WRITE LAYERS + TILES
	rapidjson::Value layersJSON(rapidjson::kObjectType);

	rapidjson::Value layer(rapidjson::kArrayType);

	tileLayer->save(layer, d.GetAllocator());
	layersJSON.AddMember("tiles", layer, d.GetAllocator());

	rapidjson::Value entJSON(rapidjson::kArrayType);

	entityLayer->save(entJSON, d.GetAllocator());
	layersJSON.AddMember("entities", entJSON, d.GetAllocator());

	d.AddMember("layers", layersJSON, d.GetAllocator());
}
TileMap& TileMap::operator = (TileMap& other){
	TileMap::width = other.width;
	TileMap::height = other.height;

	TileMap::factory = std::move(other.factory);

	tileLayer = std::move(other.tileLayer);
	entityLayer = std::move(other.entityLayer);

	return *this;
}
std::vector<sf::Vector2i> TileMap::getNeighbours(int x, int y){
	std::vector<sf::Vector2i> list;
	if(x - 1 >= 0 && tileLayer->isOccupied(x - 1, y)){
		list.push_back(sf::Vector2i(x - 1, y));
	}
	if(x + 1 < width && tileLayer->isOccupied(x + 1, y)){
		list.push_back(sf::Vector2i(x + 1, y));
	}
	if(y - 1 >= 0 && tileLayer->isOccupied(x, y - 1)){
		list.push_back(sf::Vector2i(x, y - 1));
	}
	if(y + 1 < height && tileLayer->isOccupied(x, y + 1)){
		list.push_back(sf::Vector2i(x, y + 1));
	}

	return list;
}
int TileMap::getCost(int dx, int dy){
	if(!tileLayer->isOccupied(dx, dy)) return Tile::IMPASSABLE;

	int cost = 0;
	for(auto& t : tileLayer->getTile(dx, dy)){
		cost += t->getCost();
	}

	for(EntityRefNode& e : entityLayer->getEntitiesAt(dx, dy)){
		cost += e.cost;
	}

	return cost;
}
TileLayer& TileMap::getTileLayer(){
	return *tileLayer;
}
EntityLayer& TileMap::getEntityLayer(){
	return *entityLayer;
}
std::shared_ptr<TileFactory> TileMap::getFactory(){
	return factory;
}
int TileMap::getWidth(){
	return TileMap::width;
}
int TileMap::getHeight(){
	return TileMap::height;
}
