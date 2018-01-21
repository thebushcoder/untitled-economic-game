/*
 * textureAtlas.cpp
 *
 *  Created on: 8 Feb 2016
 *      Author: linbob
 */

#include "textureAtlas.hpp"

TextureRegion::TextureRegion(rapidjson::Value& regionJson){
	std::string name = regionJson[NAME].GetString();
	size_t extStart = name.find_last_of(".");
	TextureRegion::name = name.substr(0, extStart);

	TextureRegion::area.left = regionJson[DIMENSIONS]["x"].GetInt();
	TextureRegion::area.top = regionJson[DIMENSIONS]["y"].GetInt();
	TextureRegion::area.width = regionJson[DIMENSIONS]["w"].GetInt();
	TextureRegion::area.height = regionJson[DIMENSIONS]["h"].GetInt();
}
sf::IntRect TextureRegion::getArea(){
	return TextureRegion::area;
}
std::string TextureRegion::getName(){
	return TextureRegion::name;
}
SpriteSheet::SpriteSheet(std::string filePath){
	rapidjson::Document d;
	std::string jsonPath;
	std::string imgPath;
	char readBuffer[65536];

	//LOAD JSON FILE
	jsonPath = filePath + JSONEXT;

	FILE* fp = fopen(&jsonPath[0], "rb"); // non-Windows use "r"
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);

	fclose(fp);

	SpriteSheet::regionCount = d[OBJECTSTART].Size();

	int i;
	for(i = 0; i < SpriteSheet::regionCount; i++){
		TextureRegion r = TextureRegion(d[OBJECTSTART][i]);
		SpriteSheet::regions.push_back(r);
	}

	//LOAD TEXTURE
	SpriteSheet::texture = sf::Texture();
	imgPath = filePath + PNGEXT;

	SpriteSheet::texture.loadFromFile(imgPath);
}

sf::Texture& SpriteSheet::getTexture(){
	return SpriteSheet::texture;
}
TextureRegion* SpriteSheet::getRegion(int index){
	return &SpriteSheet::regions[index];
}
int SpriteSheet::getRegionCount(){
	return SpriteSheet::regionCount;
}

TextureRegion* SpriteSheet::findRegion(std::string name){
	int i;
	for(i = 0; i < SpriteSheet::regions.size(); i++){
		TextureRegion r = SpriteSheet::regions[i];

		if(r.getName().compare(name) == 0){
			return &SpriteSheet::regions[i];
		}
	}
	return NULL;
}

TextureAtlas::TextureAtlas(std::string dirPath, std::string atlasName, int size){
	TextureAtlas::sheetCount = size;

	int i;
	for(i = 0; i < size; i++){
		/*change this to use snprintf*/
		std::string filePath;
		char countStr[4];
		sprintf(countStr, "%d", i);
		filePath = dirPath + atlasName + countStr;

		SpriteSheet s = SpriteSheet(filePath);
		TextureAtlas::sheets.push_back(s);
	}
}
std::unique_ptr<sf::Sprite> TextureAtlas::createSprite(std::string name){
	int i;
	int size = TextureAtlas::sheets.size();
	for(i = 0; i < size; i++){
		TextureRegion* r = TextureAtlas::sheets[i].findRegion(name);
		if(r != NULL){
			sf::Sprite* s = new sf::Sprite();
			s->setTexture(TextureAtlas::sheets[i].getTexture());

			s->setTextureRect(r->getArea());

			s->setOrigin(s->getLocalBounds().width / 2, s->getLocalBounds().height / 2);
			return std::unique_ptr<sf::Sprite>(s);
		}
	}

	return nullptr;
}
SpriteSheet& TextureAtlas::getSheet(int index){
	return TextureAtlas::sheets[index];
}
int TextureAtlas::getSheetCount(){
	return TextureAtlas::sheetCount;
}
