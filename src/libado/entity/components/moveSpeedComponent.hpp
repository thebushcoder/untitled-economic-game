/*
 * movementSpeed.hpp
 *
 *  Created on: 19 Aug 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_MOVESPEEDCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_MOVESPEEDCOMPONENT_HPP_

#include <anax/Component.hpp>

struct MoveSpeedComponent : anax::Component{
	float baseValue;

	MoveSpeedComponent(){}
	MoveSpeedComponent(float speed) : baseValue(speed){}
};

#endif /* LIBADO_ENTITY_COMPONENTS_MOVESPEEDCOMPONENT_HPP_ */
