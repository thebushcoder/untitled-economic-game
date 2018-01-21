/*
 * randEvent.cpp
 *
 *  Created on: 13 Mar 2017
 *      Author: linbob
 */

#include "randEvent.hpp"

RandEvent::Effect::Effect(std::string resN, float m, Attribute a) : isT(false), tgt(resN),
	modifier(m), attr(a){}

RandEvent::Effect::Effect(Item::Type t, float m, Attribute a) : isT(true), tgt(t),
	modifier(m), attr(a){}

RandEvent::RandEvent(rapidjson::Value& jsonVal){
	name = jsonVal["name"].GetString();
	desc = jsonVal["desc"].GetString();
	chance = jsonVal["chance"].GetFloat();
	duration = jsonVal["dur"].GetInt();
	fixedDuration = jsonVal["fixedDur"].GetBool();
	cooldown = jsonVal["coolD"].GetInt();

	if(jsonVal["effect"].HasMember("name")){
		effect = std::unique_ptr<RandEvent::Effect>(new RandEvent::Effect(
				jsonVal["effect"]["name"].GetString(), jsonVal["effect"]["mod"].GetFloat(),
				Effect::Attribute(jsonVal["effect"]["attr"].GetInt())));
	}else{
		effect = std::unique_ptr<RandEvent::Effect>(new RandEvent::Effect(
				Item::Type(jsonVal["effect"]["type"].GetInt()), jsonVal["effect"]["mod"].GetFloat(),
				Effect::Attribute(jsonVal["effect"]["attr"].GetInt())));
	}
}
