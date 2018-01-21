/*
 * serializableComponent.hpp
 *
 *  Created on: 29 Feb 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_SERIALIZABLECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_SERIALIZABLECOMPONENT_HPP_

#include <iostream>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filewritestream.h>

#include "../../textureAtlas.hpp"

struct SerializableComponent{
	virtual ~SerializableComponent(){}

	virtual bool save(rapidjson::Value& entData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){return true;}
	virtual bool load(rapidjson::Value& jsonData){return true;}
};



#endif /* LIBADO_ENTITY_COMPONENTS_SERIALIZABLECOMPONENT_HPP_ */
