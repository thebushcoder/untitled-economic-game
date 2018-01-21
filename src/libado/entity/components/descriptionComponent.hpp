/*
 * descriptionComponent.hpp
 *
 *  Created on: 2 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_DESCRIPTIONCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_DESCRIPTIONCOMPONENT_HPP_

#include <anax/Component.hpp>

/*	Used to tag items
 * 	tag only class
 */
struct DescriptionComponent : anax::Component{
	DescriptionComponent(std::string desc) : value(desc){}
	std::string getDescription(){
		return value;
	}
private:
	std::string value;
};



#endif /* LIBADO_ENTITY_COMPONENTS_DESCRIPTIONCOMPONENT_HPP_ */
