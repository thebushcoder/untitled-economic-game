/*
 * walkToTgt.hpp
 *
 *  Created on: 28 Aug 2016
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../../map/zone.hpp"

namespace bt
{

class WalkToTgt : public Leaf
{
public:
	WalkToTgt(Blackboard::Ptr board, Overworld* overWorld) : Leaf(board),
		overWorld(overWorld){}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	privBoard->SetInt("tile_cost", 0);
    	AiComponent& ai = e.getComponent<AiComponent>();
		PositionComponent& p = e.getComponent<PositionComponent>();

		sf::Vector2i next = ai.path.back();

		//remove entity from previous tile
		overWorld->getActiveZone()->zone->getEntityLayer().removeEntity(p.tilePosition.x,
				p.tilePosition.y, e.getId().index);

		//reposition entity
		p.setPosition(next.x, next.y, next.x * TILESIZE, next.y * TILESIZE);

		//add entity to tile
		overWorld->getActiveZone()->zone->getEntityLayer().setEntity(next.x, next.y, e);

		ai.path.pop_back();

		int cost = overWorld->getActiveZone()->zone->getCost(next.x, next.y);
		privBoard->SetInt("tile_cost", cost);

		if(!ai.path.empty()){
			return Status::Running;
		}else{
			return Status::Success;
		}
    }

protected:
    Overworld* overWorld;
};

}
