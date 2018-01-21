/*
 * itemTypes.hpp
 *
 *  Created on: 2 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ITEMTYPES_HPP_
#define LIBADO_ENTITY_COMPONENTS_ITEMTYPES_HPP_

namespace Item{
	enum Type{
		WEAPON,
		TOOL,
		COMMODITY,
		CONSUMABLE,
		VALUABLE,
		ONE_HAND,
		TWO_HAND,
		MELEE,
		RANGED,
		SHIELD,
		FOOD,
		MISC,
		DRUG,
		RAW_GOOD,
		PROCESSED_GOOD,
		SPECIALITY_GOOD,
		QUEST,
		RING,
		NECKLACE,
		WEARABLE,
		MOUNT,
		OUTFIT,
		TRINKET,
		TOKEN,
		MAIN_HAND,
		OFF_HAND,
		ARMOUR
	};
	static std::string enumToString(Type t){
		switch(t){
		case WEAPON:
			return "WEAPON";
			break;
		case TOOL:
			return "TOOL";
			break;
		case COMMODITY:
			return "COMMODITY";
			break;
		case CONSUMABLE:
			return "CONSUMABLE";
			break;
		case VALUABLE:
			return "VALUABLE";
			break;
		case ARMOUR:
			return "ARMOUR";
			break;
		case MELEE:
			return "MELEE";
			break;
		case RANGED:
			return "RANGED";
			break;
		case FOOD:
			return "FOOD";
			break;
		case MISC:
			return "MISC";
			break;
		case DRUG:
			return "DRUG";
			break;
		case RAW_GOOD:
			return "INGREDIENT";
			break;
		case QUEST:
			return "QUEST";
			break;
		case RING:
			return "RING";
			break;
		case NECKLACE:
			return "NECKLACE";
			break;
		case WEARABLE:
			return "WEARABLE";
			break;
		case MOUNT:
			return "MOUNT";
			break;
		case ONE_HAND:
			return "ONE_HAND";
			break;
		case TWO_HAND:
			return "TWO_HAND";
			break;
		case TRINKET:
			return "TRINKET";
			break;
		case MAIN_HAND:
			return "ONE_HAND";
			break;
		case OFF_HAND:
			return "OFF_HAND";
			break;
		case TOKEN:
			return "TOKEN";
			break;
		case OUTFIT:
			return "OUTFIT";
			break;
		case PROCESSED_GOOD:
			return "PROCESSED_GOOD";
			break;
		case SPECIALITY_GOOD:
			return "SPECIALITY_GOOD";
			break;
		}
		return "";
	}
}

#endif /* LIBADO_ENTITY_COMPONENTS_ITEMTYPES_HPP_ */
