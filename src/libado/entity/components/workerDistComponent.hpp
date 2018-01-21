/*
 * workerDistComponent.hpp
 *
 *  Created on: 2 Feb 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_WORKERDISTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_WORKERDISTCOMPONENT_HPP_

#include <anax/Component.hpp>

/*
 * Worker distribution. % of workers allocated to producing a particular item/resource
 */
struct WorkerDistComponent : anax::Component{
	float value;

	WorkerDistComponent(){}

	WorkerDistComponent(float dist) : value(dist){}
};

#endif /* LIBADO_ENTITY_COMPONENTS_WORKERDISTCOMPONENT_HPP_ */
