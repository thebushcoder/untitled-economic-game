/*
 * findPath.hpp
 *
 *  Created on: 2 Sep 2016
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../../util/aStar.hpp"

namespace bt
{

//walk to the exact tile tgt is standing on
class PathToTgt : public Leaf
{
public:
	PathToTgt(Overworld* overWorld, Blackboard::Ptr board) :
			Leaf(board), overWorld(overWorld){}

	void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
	{
		if(privBoard->HasInt("tgt_entity")){
			PositionComponent& tgtPos = e.getWorld().getEntity(privBoard->GetInt("tgt_entity")).getComponent<PositionComponent>();
			privBoard->SetVector("tgt_tile", sf::Vector2i(tgtPos.tilePosition.x, tgtPos.tilePosition.y));
		}
	}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    	if(!privBoard->HasVector("tgt_tile")) return Status::Failure;
    	sf::Vector2i& tgtTile = privBoard->GetVector("tgt_tile");

    	PositionComponent& entPos = e.getComponent<PositionComponent>();

//    	printf("\t\t#FIND PATH TO TGT\n");

    	if((tgtTile != entPos.tilePosition)){
			AiComponent& ai = e.getComponent<AiComponent>();

			//find path
			std::vector<sf::Vector2i> path = AStar::findPath(*overWorld->getActiveZone()->zone,
					AStar::GraphNode(entPos.tilePosition.x, entPos.tilePosition.y),
					AStar::GraphNode(tgtTile.x, tgtTile.y));

			if(path.empty()){
				privBoard->RemoveVector("tgt_tile");
				return Status::Failure;
			}

			ai.path = path;
			privBoard->RemoveVector("tgt_tile");
			return Status::Success;
    	}
    	privBoard->RemoveVector("tgt_tile");
		return Status::Failure;
    }
private:
    Overworld* overWorld;
};

}
