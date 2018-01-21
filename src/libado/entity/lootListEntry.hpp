/*
 * lootListEntry.hpp
 *
 *  Created on: 19 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_LOOTLISTENTRY_HPP_
#define LIBADO_ENTITY_LOOTLISTENTRY_HPP_

#include <rapidjson/document.h>
#include <regex>

struct LootListEntry{
	std::string name;	//item name or catagory name
	float dropChance;	// between 0.0f-1.0f
	int minLevel = 0, maxLevel = 99;
	int minCount = 1, maxCount = 2; //min/maxCount = number of loot items to drop
	int dropLimit; //max times an item can be spawned. quest/unique items

	LootListEntry(){}

	LootListEntry(rapidjson::Value& jsonData){
		name = jsonData["name"].GetString();
		dropChance = jsonData["dropChance"].GetFloat();
		if(jsonData.HasMember("minLevel")){
			minLevel = jsonData["minLevel"].GetInt();
		}
		if(jsonData.HasMember("maxLevel")){
			maxLevel = jsonData["maxLevel"].GetInt();
		}
		if(jsonData.HasMember("minCount")){
			minCount = jsonData["minCount"].GetInt();
		}
		if(jsonData.HasMember("maxCount")){
			maxCount = jsonData["maxCount"].GetInt();
		}
		if(jsonData.HasMember("dropLimit")){
			dropLimit = jsonData["dropLimit"].GetInt();
		}
	}

	bool isCatagory(){
		std::regex groupRegex("^\\([a-zA-Z]*\\)$");

		return std::regex_match(name, groupRegex);
	}
};



#endif /* LIBADO_ENTITY_LOOTLISTENTRY_HPP_ */
