/*
 * OwnerComponent.hpp
 *
 *  Created on: 11 Jul 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_OWNERCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_OWNERCOMPONENT_HPP_

#include <anax/Component.hpp>
#include "serialComponent.hpp"

/*
 * Notes an entity(mount)'s owner. Eg : player owns a mount(as opposed to a mount thats free;to roam)
 */
struct OwnerComponent : anax::Component, SerializableComponent{
	anax::Entity owner;
	bool isPlayer = false;

	OwnerComponent(anax::Entity e) : owner(e){}

	anax::Entity& getOwner(){
		return owner;
	}
	bool ownerIsPlayer(){
		return isPlayer;
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_OWNERCOMPONENT_HPP_ */
