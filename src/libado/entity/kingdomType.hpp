/*
 * kingdomType.hpp
 *
 *  Created on: 26 Dec 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_KINGDOMTYPE_HPP_
#define LIBADO_ENTITY_KINGDOMTYPE_HPP_

namespace KingdomUtil{
	enum KingdomType{
		NONE,	// default/null option
		HUMAN,
		ELF,
		DWARF,
		REPUB
	};

	static std::string TypetoString(KingdomType t){
		switch(t){
		case HUMAN:
			return "Human";
			break;
		case ELF:
			return "Elf";
			break;
		case DWARF:
			return "Dwarf";
			break;
		case REPUB:
			return "Republic";
			break;
		case NONE:
			return "None";
		}
		return "";
	}
}

#endif /* LIBADO_ENTITY_KINGDOMTYPE_HPP_ */
