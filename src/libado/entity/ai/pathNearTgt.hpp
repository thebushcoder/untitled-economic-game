/*
 * pathNearTgt.hpp
 *
 *  Created on: 8 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../../util/aStar.hpp"

namespace bt
{

//walk to tile within 3x3 grid of tgt
class PathNearTgt : public Leaf
{
public:
	PathNearTgt(Overworld* overWorld, Blackboard::Ptr board) :
			Leaf(board), overWorld(overWorld){}

	void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
	{
		//find an unoccupied tile next to tgt
		if(privBoard->HasInt("tgt_entity")){
			PositionComponent& tgtPos = e.getWorld().getEntity(
					privBoard->GetInt("tgt_entity")).getComponent<PositionComponent>();
			TileMap* map = overWorld->getActiveZone()->zone.get();
			if(map->getCost(tgtPos.tilePosition.x + 1, tgtPos.tilePosition.y) < Tile::IMPASSABLE){
				privBoard->SetVector("tgt_tile",
						sf::Vector2i(tgtPos.tilePosition.x + 1, tgtPos.tilePosition.y));
			}else if(map->getCost(tgtPos.tilePosition.x - 1, tgtPos.tilePosition.y) < Tile::IMPASSABLE){
				privBoard->SetVector("tgt_tile",
						sf::Vector2i(tgtPos.tilePosition.x - 1, tgtPos.tilePosition.y));
			}else if(map->getCost(tgtPos.tilePosition.x, tgtPos.tilePosition.y + 1) < Tile::IMPASSABLE){
				privBoard->SetVector("tgt_tile",
						sf::Vector2i(tgtPos.tilePosition.x, tgtPos.tilePosition.y + 1));
			}else if(map->getCost(tgtPos.tilePosition.x, tgtPos.tilePosition.y - 1) < Tile::IMPASSABLE){
				privBoard->SetVector("tgt_tile",
						sf::Vector2i(tgtPos.tilePosition.x, tgtPos.tilePosition.y - 1));
			}
		}
	}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    	if(!privBoard->HasVector("tgt_tile")) return Status::Failure;

    	sf::Vector2i& tgtTile = privBoard->GetVector("tgt_tile");

    	PositionComponent& entPos = e.getComponent<PositionComponent>();

    	if(tgtTile != entPos.tilePosition){
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
