/*
 * eventSystem.hpp
 *
 *  Created on: 13 Mar 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_EVENTSYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_EVENTSYSTEM_HPP_

#include <anax/System.hpp>
#include <random>

#include "../components/eventListComponent.hpp"

struct EventSystem : public anax::System<anax::Requires<EventListComponent>>{

	EventSystem(){
		std::random_device rd;     // only used once to initialise (seed) engine
		rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	}
	void process(){

//		std::uniform_int_distribution<int> uni(min,max); // guaranteed unbiased
//
//		auto random_integer = uni(rng);
	}

private:
	std::mt19937 rng;
};

#endif /* LIBADO_ENTITY_SYSTEMS_EVENTSYSTEM_HPP_ */
