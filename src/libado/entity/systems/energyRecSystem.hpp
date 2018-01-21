/*
 * energyRecSystem.hpp
 *
 *  Created on: 19 Aug 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_ENERGYRECSYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_ENERGYRECSYSTEM_HPP_

#include <anax/System.hpp>

#include "../components/energyComponent.hpp"

struct EnergyRecSystem : anax::System<anax::Requires<EnergyComponent>>{

	EnergyRecSystem(EntityFactory* f) : factory(f){}

	void process(sf::Time delta){
		EnergyComponent& playerEnergy = factory->getPlayer().getComponent<EnergyComponent>();

		//waiting for player to make a move. halt all processing
		if(playerEnergy.count >= AiSystem::stdTurnCost) return;

		for(anax::Entity e : getEntities()){
			EnergyComponent& energy = e.getComponent<EnergyComponent>();

			energy.count += energy.energyRecovery;
		}
	}

private:
	EntityFactory* factory;
};

#endif /* LIBADO_ENTITY_SYSTEMS_ENERGYRECSYSTEM_HPP_ */
