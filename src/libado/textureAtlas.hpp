/*
 * textureAtlas.h
 *
 *  Created on: 8 Feb 2016
 *      Author: linbob
 */

#ifndef TEXTUREATLAS_HPP_
#define TEXTUREATLAS_HPP_

#include <cstdio>
#include <vector>
#include <string>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#define OBJECTSTART "frames"
#define NAME "filename"
#define DIMENSIONS "frame"
#define JSONEXT ".json"
#define PNGEXT ".png"

class TextureRegion{
public:
	TextureRegion(rapidjson::Value& regionJson);

	std::string getName();
	sf::IntRect getArea();
private:
	sf::IntRect area;
	std::string name;
};

class SpriteSheet{
public:
	SpriteSheet(std::string filePath);

	int getRegionCount();
	sf::Texture& getTexture();
	TextureRegion* getRegion(int index);
	TextureRegion* findRegion(std::string name);
protected:
	sf::Texture texture;
	int regionCount;
	std::vector<TextureRegion> regions;
};

class TextureAtlas{
public:
	TextureAtlas(){}
	TextureAtlas(std::string dirPath, std::string atlasName, int size);

	std::unique_ptr<sf::Sprite> createSprite(std::string name);
	SpriteSheet& getSheet(int index);
	int getSheetCount();
private:
	int sheetCount;
	std::vector<SpriteSheet> sheets;
};

#endif /* TEXTUREATLAS_HPP_ */
