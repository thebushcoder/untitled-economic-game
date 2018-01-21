/*
 * zoneComponent.hpp
 *
 *  Created on: 2 Dec 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_ZONECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_ZONECOMPONENT_HPP_

#include <anax/Component.hpp>

struct ZoneComponent : anax::Component{
	std::string zoneName;

	ZoneComponent(){}
	ZoneComponent(std::string z) : zoneName(z){}
};

#endif /* LIBADO_ENTITY_COMPONENTS_ZONECOMPONENT_HPP_ */
