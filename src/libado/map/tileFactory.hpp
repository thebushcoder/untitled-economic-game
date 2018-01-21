/*
 * tileFactory.hpp
 *
 *  Created on: 25 Feb 2016
 *      Author: linbob
 */

#ifndef TILEFACTORY_HPP_
#define TILEFACTORY_HPP_

#include <vector>
#include <string>
#include <map>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "../textureAtlas.hpp"

#define TILESIZE 32

class Tile{
public:
	static constexpr int IMPASSABLE = 9999;

	Tile() : tileSprite(std::move(std::unique_ptr<sf::Sprite>(nullptr))),
		body(std::move(std::unique_ptr<sf::RectangleShape>(nullptr))){}
	Tile(int i, int c, std::unique_ptr<sf::Sprite>& s, std::unique_ptr<sf::RectangleShape>& b);

	sf::RectangleShape& getBody();
	sf::Sprite& getTileSprite();
	int getId();
	int getCost();
	void setPosition(int x, int y);
	void copyTile(Tile* t);
protected:
	int id = 0;
	int cost = 0;
	std::unique_ptr<sf::Sprite> tileSprite;
	std::unique_ptr<sf::RectangleShape> body;
};

class TileTemplate{
public:
	TileTemplate(rapidjson::Value& tileData);

	std::unique_ptr<Tile> createTile(std::shared_ptr<TextureAtlas>& atlas);
	std::string& getSprite(){
		return spriteName;
	}
	int getId(){
		return id;
	}
	int getCost(){
		return cost;
	}
protected:
	int id, cost;
	bool hasBody = false;
	std::string spriteName;
};

class TileFactory{
public:
	TileFactory(){}
	TileFactory(std::string jsonPath, std::shared_ptr<TextureAtlas>& atlas);

	std::unique_ptr<Tile> createTile(int id);
	TileTemplate* getTileTemplate(int id);
	std::map<int, std::unique_ptr<TileTemplate>>& getTiles();
	TextureAtlas& getAtlas(){
		return *atlas;
	}
private:
	std::map<int, std::unique_ptr<TileTemplate>> sourceTiles;
	std::shared_ptr<TextureAtlas> atlas;
};


#endif /* TILEFACTORY_HPP_ */
