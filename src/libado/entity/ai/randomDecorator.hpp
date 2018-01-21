/*
 * randomDecorator.hpp
 *
 *  Created on: 14 Aug 2016
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class RandomDecorator : public Decorator
{
public:
	RandomDecorator(Blackboard::Ptr board, int chance) :
		Decorator(board), chance(chance) {
		srand((int)time(0));
	}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	float value = rand() % 100;

		if(value <= chance){
			return child->Tick(e, privBoard);
		}
		return Status::Failure;
    }

protected:
    int chance;
};

}
