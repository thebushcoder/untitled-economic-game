#pragma once

#include <random>
#include <chrono>
#include "../../util/bt-master/bt.hpp"
#include "../../util/aStar.hpp"

namespace bt
{

//needs to be random range (current - diff, current + diff)
class TgtRandomTile : public Leaf
{
private:
	Overworld* overWorld;
    int maxDist = 1;
    std::mt19937 gen;
public:
	TgtRandomTile(int maxDist, Overworld* overWorld, Blackboard::Ptr board) :
		Leaf(board), overWorld(overWorld), maxDist(maxDist){
		gen = std::mt19937(std::chrono::system_clock::now().
				time_since_epoch() / std::chrono::milliseconds(1));
	}

	void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
	{
	}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    	sf::Vector2i tgtTile;
    	TileMap* map = overWorld->getActiveZone()->zone.get();
    	PositionComponent& entPos = e.getComponent<PositionComponent>();

//    	printf("\t\t#TGT RANDOM TILE\n");

    	bool found = false;
    	while(!found){
			std::uniform_int_distribution<int> xDist(entPos.tilePosition.x - maxDist,
					entPos.tilePosition.x + maxDist);
			std::uniform_int_distribution<int> yDist(entPos.tilePosition.y - maxDist,
					entPos.tilePosition.y + maxDist);

			tgtTile.x = xDist(gen);
			tgtTile.y = yDist(gen);

			//tgt inside bounds
			bool t1 = (tgtTile.x < 0 || tgtTile.x >= map->getWidth() ||
	    			tgtTile.y < 0 || tgtTile.y >= map->getHeight());

			if(t1) continue;

			//tile is null
			bool t2 = !map->getTileLayer().isOccupied(tgtTile.x, tgtTile.y);

			if(t2) continue;

			//check tile pos & cost
			bool t3 = tgtTile == entPos.tilePosition;
			bool t4 = map->getCost(tgtTile.x, tgtTile.y) >= Tile::IMPASSABLE;

			if(!t3 && !t4){
//				printf("TgtRandomTile : %d, %d\n", tgtTile.x, tgtTile.y);
				found = true;
			}
    	}

    	privBoard->SetVector("tgt_tile", tgtTile);

    	return Status::Success;
    }

};

}
