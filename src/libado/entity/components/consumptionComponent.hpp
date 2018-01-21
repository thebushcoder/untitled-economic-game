/*
 * consumptionComponent.hpp
 *
 *  Created on: 2 Jan 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_CONSUMPTIONCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_CONSUMPTIONCOMPONENT_HPP_

#include <anax/Component.hpp>
#include <rapidjson/document.h>
#include "itemTypes.hpp"

struct ConsumptionUnit{
	Item::Type type;
	float mainQty;
	float secondaryQty;
	float itemQuality;

	ConsumptionUnit(Item::Type t, float m, float second = 0, float quality = 0) :
		type(t), mainQty(m), secondaryQty(second), itemQuality(0){}
};

struct ConsumptionComponent : anax::Component{
	std::vector<ConsumptionUnit> consumedResources;

	ConsumptionComponent(){}

	ConsumptionComponent(rapidjson::Value& jsonData){
		for(auto& v : jsonData.GetArray()){
			ConsumptionUnit unit = ConsumptionUnit(Item::Type(v["type"].GetInt()), v["main"].GetFloat());
			if(v.HasMember("second")){
				unit.secondaryQty = v["second"].GetFloat();
			}
			if(v.HasMember("quality")){
				unit.itemQuality = v["quality"].GetFloat();
			}
			consumedResources.push_back(unit);
		}
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_CONSUMPTIONCOMPONENT_HPP_ */
