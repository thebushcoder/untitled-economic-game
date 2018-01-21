/*
 * hasPath.hpp
 *
 *  Created on: 9 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class HasPath : public Decorator
{
public:
	HasPath(Blackboard::Ptr board) :
		Decorator(board){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    bool conditionCheck(anax::Entity& e, Blackboard::Ptr privBoard){
    	AiComponent& ai = e.getComponent<AiComponent>();
		//ensure there is an existing path
		if(ai.path.empty()){
			return false;
		}else{
			return true;
		}
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	return child->Tick(e, privBoard);
    }
};

}
