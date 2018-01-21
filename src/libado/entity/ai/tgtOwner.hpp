/*
 * tgtOwner.hpp
 *
 *  Created on: 15 Jul 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../components/ownerComponent.hpp"

namespace bt
{

class TgtOwner : public Decorator
{
public:
	TgtOwner(Blackboard::Ptr board) :
		Decorator(board){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	OwnerComponent& owner = e.getComponent<OwnerComponent>();

		privBoard->SetInt("tgt_entity", owner.owner.getId().index);
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
		return child->Tick(e, privBoard);
    }
};

}
