/*
 * tileFactory.cpp
 *
 *  Created on: 25 Feb 2016
 *      Author: linbob
 */

#include "tileFactory.hpp"

Tile::Tile(int i, int c, std::unique_ptr<sf::Sprite>& s, std::unique_ptr<sf::RectangleShape>& b)
	:id(i), cost(c), tileSprite(std::move(s)), body(std::move(b)){}

sf::RectangleShape& Tile::getBody(){
	return *Tile::body;
}
sf::Sprite& Tile::getTileSprite(){
	return *Tile::tileSprite;
}
int Tile::getId(){
	return Tile::id;
}
int Tile::getCost(){
	return Tile::cost;
}
void Tile::setPosition(int x, int y){
	sf::Vector2f pos(x, y);
	tileSprite->setPosition(pos);

	if(body != NULL){
		Tile::body->setPosition(pos);
	}
}
void Tile::copyTile(Tile* t){
	id = t->id;
	cost = t->cost;
	sf::Sprite* s = new sf::Sprite(*t->tileSprite->getTexture(),
				t->tileSprite->getTextureRect());
	tileSprite.reset(s);
	body.reset(new sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE)));
}
TileTemplate::TileTemplate(rapidjson::Value& tileData){
	TileTemplate::id = tileData["id"].GetInt();
	TileTemplate::cost = tileData["cost"].GetInt();
	TileTemplate::spriteName = tileData["sprite"].GetString();
	if(tileData.HasMember("body")){
		TileTemplate::hasBody = true;
	}
}
std::unique_ptr<Tile> TileTemplate::createTile(std::shared_ptr<TextureAtlas>& atlas){
	std::unique_ptr<sf::Sprite> sprite = atlas->createSprite(spriteName);
	std::unique_ptr<sf::RectangleShape> tileBody = nullptr;
	if(hasBody){
		tileBody = std::unique_ptr<sf::RectangleShape>(
				new sf::RectangleShape(sf::Vector2f(TILESIZE, TILESIZE))
		);
	}

	return std::unique_ptr<Tile>(
			new Tile(TileTemplate::id, TileTemplate::cost, sprite, tileBody)
	);
}
TileFactory::TileFactory(std::string jsonPath, std::shared_ptr<TextureAtlas>& atlas){
	char readBuffer[65536];
	rapidjson::Document d;

	TileFactory::atlas = atlas;

	FILE* fp = fopen(&jsonPath[0], "r");
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);

	fclose(fp);

	for(int i = 0; i < d["tiles"].Size(); ++i){
		int id = d["tiles"][i]["id"].GetInt();

		TileFactory::sourceTiles.insert(std::make_pair(id,
				std::unique_ptr<TileTemplate>(new TileTemplate(d["tiles"][i])))
		);
	}
}
std::unique_ptr<Tile> TileFactory::createTile(int id){
	return TileFactory::getTileTemplate(id)->createTile(atlas);
}
TileTemplate* TileFactory::getTileTemplate(int id){
	std::map<int, std::unique_ptr<TileTemplate>>::iterator iter = TileFactory::sourceTiles.find(id);
	if (iter != TileFactory::sourceTiles.end() ){
	    // key 2 exists, do something with iter->second (the value)
		return iter->second.get();
	}
	return nullptr;
}
std::map<int, std::unique_ptr<TileTemplate>>& TileFactory::getTiles(){
	return sourceTiles;
}
