/*
 * turnLoop.hpp
 *
 *  Created on: 25 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt/Decorator.hpp"

namespace bt
{

class TurnLoop : public Decorator
{
public:
	TurnLoop(Blackboard::Ptr board) : Decorator(board) {}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	NameComponent& name = e.getComponent<NameComponent>();
    	EnergyComponent& energy = e.getComponent<EnergyComponent>();

    	Status s;
    	//loop tree until failure or energy depletion
    	while(energy.count >= AiSystem::stdTurnCost){
    		s = child->Tick(e, privBoard);
    		if(s == Status::Failure){
    			return Status::Failure;
    		}
    	}

    	return s;
    }
};

}
