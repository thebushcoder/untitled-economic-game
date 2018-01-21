/*
 * chkTgtLocation.hpp
 *
 *  Created on: 19 Nov 2016
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{
//	Check if entity is next to target - continue only if entity not near tgt
class ChkTgtDistance : public Decorator
{
public:
	ChkTgtDistance(bool nearT, Overworld* overWorld, Blackboard::Ptr board) :
		Decorator(board), nearTgt(nearT), overWorld(overWorld){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    bool conditionCheck(anax::Entity& e, Blackboard::Ptr privBoard){
    	int tgt = privBoard->GetInt("tgt_entity");

		PositionComponent& tgtPos = e.getWorld().getEntity(tgt).getComponent<PositionComponent>();
		PositionComponent& playerPos = e.getComponent<PositionComponent>();

		if(nearTgt){
			if((tgtPos.tilePosition.x >= playerPos.tilePosition.x - 1 &&
				tgtPos.tilePosition.x <= playerPos.tilePosition.x + 1) &&
				(tgtPos.tilePosition.y >= playerPos.tilePosition.y - 1 &&
				tgtPos.tilePosition.y <= playerPos.tilePosition.y + 1)){
				//	entity is within 3x3 grid around tgt - do not execute child action
				return false;
			}
		}else{
			if(tgtPos.tilePosition.x == playerPos.tilePosition.x &&
					tgtPos.tilePosition.y == playerPos.tilePosition.y){
				return false;
			}
		}
		//	entity not near tgt - execute child action
		return true;
	}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
			return child->Tick(e, privBoard);
    }
protected:
    bool nearTgt = true;
    Overworld* overWorld;
};

}
