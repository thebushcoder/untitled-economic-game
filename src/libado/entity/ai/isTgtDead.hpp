/*
 * isTgtDead.hpp
 *
 *  Created on: 9 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class IsTgtDead : public Decorator
{
public:
	IsTgtDead(Blackboard::Ptr board) :
		Decorator(board){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	//is tgt dead?
		int tgt = privBoard->GetInt("tgt_entity");

		if(!e.getWorld().getEntity(tgt).isValid()){
			privBoard->RemoveInt("tgt_entity");
			return Status::Failure;
		}else{
			return child->Tick(e, privBoard);
		}
    }
};

}
