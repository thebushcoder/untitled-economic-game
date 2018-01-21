/*
 * weightComponent.hpp
 *
 *  Created on: 2 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_WEIGHTCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_WEIGHTCOMPONENT_HPP_

#include <anax/Component.hpp>

struct WeightComponent : anax::Component{
	WeightComponent(float w) : weight(w){}
	float getWeight(){
		return weight;
	}
private:
	float weight;
};



#endif /* LIBADO_ENTITY_COMPONENTS_WEIGHTCOMPONENT_HPP_ */
