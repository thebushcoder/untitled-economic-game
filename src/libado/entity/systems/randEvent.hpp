/*
 * RandEvent.hpp
 *
 *  Created on: 13 Mar 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_RANDEVENT_HPP_
#define LIBADO_ENTITY_SYSTEMS_RANDEVENT_HPP_

#include <string>
#include <memory>
#include <rapidjson/document.h>

#include "../components/itemTypes.hpp"

struct RandEvent{
	struct Effect{
		enum Attribute{
			PRICE,
			PRODUCTION,
			SUPPLY
		};

		Effect(){}
		Effect(std::string resN, float m, Attribute a);
		Effect(Item::Type t, float m, Attribute a);

		bool isType(){
			return isT;
		}
		std::string& getResName(){
			return tgt.resName;
		}
		Item::Type& getItemType(){
			return tgt.type;
		}
		float getModifier(){
			return modifier;
		}
		Attribute& getAttribute(){
			return attr;
		}
	private:
		bool isT;
		union Target{
			std::string resName;
			Item::Type type;

			Target(){}
			Target(std::string s) : resName(s){}
			Target(Item::Type t) : type(t){}
			~Target(){
				resName.~basic_string();
			}
		} tgt;

		float modifier;
		Attribute attr;
	};

	RandEvent(rapidjson::Value& jsonVal);

	std::string& getName(){
		return name;
	}
	std::string& getDescription(){
		return desc;
	}
	float getChance(){
		return chance;
	}
	int getDuration(){
		return duration;
	}
	bool isFixedDuration(){
		return fixedDuration;
	}
	int getCooldown(){
		return cooldown;
	}
	Effect* getEffect(){
		return effect.get();
	}
private:
	std::string name;
	std::string desc;
	float chance; //0.0 - 1.0
	int duration; //in turns; 0 = instant
	bool fixedDuration; //false = select duration between 0-duration; true = (duration = duration)

	int cooldown; //in turns

	std::unique_ptr<Effect> effect = nullptr;
};



#endif /* LIBADO_ENTITY_SYSTEMS_RANDEVENT_HPP_ */
