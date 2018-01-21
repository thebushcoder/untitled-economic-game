/*
 * ownerChkDecorator.hpp
 *
 *  Created on: 13 Jul 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../components/ownerComponent.hpp"
#include "../../util/fileLogger.hpp"

namespace bt
{

class HasOwner : public Decorator
{
public:
	HasOwner(bool f, Blackboard::Ptr board) :
		Decorator(board), flag(f){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    bool conditionCheck(anax::Entity& e, Blackboard::Ptr privBoard){
    	if(e.hasComponent<OwnerComponent>() == flag){
			return true;
		}else{
			return false;
		}
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
		return child->Tick(e, privBoard);
    }
protected:
    bool flag;
};

}
