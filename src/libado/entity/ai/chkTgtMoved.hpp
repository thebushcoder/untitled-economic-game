/*
 * chkTgtMoved.hpp
 *
 *  Created on: 8 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class ChkTgtMoved : public Decorator
{
public:
	ChkTgtMoved(Overworld* overWorld, Blackboard::Ptr board) :
		Decorator(board), overWorld(overWorld){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    bool conditionCheck(anax::Entity& e, Blackboard::Ptr privBoard){
    	int tgt = privBoard->GetInt("tgt_entity");
		AiComponent& ai = e.getComponent<AiComponent>();
		PositionComponent& tgtPos = e.getWorld().getEntity(tgt).getComponent<PositionComponent>();

		if(ai.path.begin()->x != tgtPos.tilePosition.x &&
				ai.path.begin()->y != tgtPos.tilePosition.y){
			//tgt has moved - find new path
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
    Overworld* overWorld;
};

}
