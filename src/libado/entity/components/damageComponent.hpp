/*
 * damageComponent.hpp
 *
 *  Created on: 19 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_DAMAGECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_DAMAGECOMPONENT_HPP_

#include <anax/Component.hpp>

/*
 * ADD:
 * dmg types
 */
struct DamageComponent : anax::Component{
	int value = 0;

	DamageComponent(){}
	DamageComponent(int dmg) : value(dmg){}
};

#endif /* LIBADO_ENTITY_COMPONENTS_DAMAGECOMPONENT_HPP_ */
