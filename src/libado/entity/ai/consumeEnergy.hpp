/*
 * consumeEnergy.hpp
 *
 *  Created on: 15 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class ConsumeEnergy : public Decorator
{
public:
	ConsumeEnergy(int energyOverworld, int energyZone, Overworld* oWorld, Blackboard::Ptr board) :
		Decorator(board), energyCost_o(energyOverworld), energyCost_z(energyZone), overWorld(oWorld){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	AiComponent& ai = e.getComponent<AiComponent>();
		EnergyComponent& energy = e.getComponent<EnergyComponent>();

    	Status s = child->Tick(e, privBoard);

    	if(overWorld->isChildActive()){
			//CONSUME ENTITY ENERGY
			energy.count -= energyCost_z;
		}else{
			//CONSUME ENTITY ENERGY - MOVEMENT IN OVERWORLD COSTS MORE PER TILE
			//2 production ticks per overworld tile; 3.5 tiles = 1 day(human walking speed{
			//mounted on horse/wagon will move faster, therefore more tiles per day})
			energy.count -= energyCost_o;
		}

    	return s;
    }
private:
    int energyCost_o, energyCost_z;
    Overworld* overWorld;
};

}
