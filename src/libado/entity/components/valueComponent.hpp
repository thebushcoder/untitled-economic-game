/*
 * valueComponent.hpp
 *
 *  Created on: 2 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_VALUECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_VALUECOMPONENT_HPP_

#include <anax/Component.hpp>

/*	Used to tag items
 * 	tag only class
 */
struct ValueComponent : anax::Component{
	ValueComponent(int v) : value(v){}
	int getValue(){
		return value;
	}
private:
	int value;
};

#endif /* LIBADO_ENTITY_COMPONENTS_VALUECOMPONENT_HPP_ */
