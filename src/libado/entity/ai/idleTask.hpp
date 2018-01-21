/*
 * idleTask.hpp
 *
 *  Created on: 14 Aug 2016
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class Idle : public Leaf
{
public:
	Idle(int maxWait, Blackboard::Ptr board) : Leaf(board), maxIdle(maxWait){
		srand((int)time(0));
	}

	void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
	{
		count = 0;
		idleTime = rand() % maxIdle + 1;
	}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    	if(e.hasComponent<PlayerComponent>()){
			if(!privBoard->GetBool("idleTurn"))return Status::Failure;
		}

//    	printf("\t\t#IDLE\n");

    	EnergyComponent& energy = e.getComponent<EnergyComponent>();
		//CONSUME ENTITY ENERGY
		energy.count -= 100;

    	if(maxIdle == 1){
    		if(e.hasComponent<PlayerComponent>()){
    			privBoard->SetBool("idleTurn", false);
    		}
    		return Status::Success;
    	}else if(++count >= idleTime){
    		if(e.hasComponent<PlayerComponent>()){
    			privBoard->SetBool("idleTurn", false);
    		}
    		return Status::Success;
    	}

		return Status::Running;
    }
private:
    int count = 0;
    int idleTime = -1;
    int maxIdle = 1;
};

}
