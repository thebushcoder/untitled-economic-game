/*
 * lootList.hpp
 *
 *  Created on: 19 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_LOOTLIST_HPP_
#define LIBADO_ENTITY_LOOTLIST_HPP_

#include "lootListEntry.hpp"

struct LootList{
	std::vector<LootListEntry> entries;

	void load(rapidjson::Value& json){
		for(auto& j : json.GetArray()){
			entries.push_back(LootListEntry(j));
		}
	}
};

#endif /* LIBADO_ENTITY_LOOTLIST_HPP_ */
