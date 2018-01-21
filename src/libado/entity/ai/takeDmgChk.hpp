/*
 * takeDmgChk.hpp
 *
 *  Created on: 19 Nov 2016
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class TakeDmgChk : public Decorator
{
public:
	TakeDmgChk(Blackboard::Ptr board) :
		Decorator(board){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
		if(!e.hasComponent<DamageComponent>()){
			return child->Tick(e, privBoard);
		}
		return Status::Failure;
    }

};

}
