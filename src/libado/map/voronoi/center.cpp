/*
 * center.cpp
 *
 *  Created on: 13 Dec 2017
 *      Author: linbob
 */

#include "center.hpp"
#include "celledge.hpp"

namespace VoronoiMap{
	CellEdge* Center::getNextVorEdge(std::vector<CellEdge*>* visited, sf::Vector2f nextPoint){
		for(auto e : polyEdges){
			if(std::find(visited->begin(), visited->end(), e.get()) != visited->end()) continue;

			CellCorner* vEdgeCorner1 = e->getVoronoiEdge().first.get();
			CellCorner* vEdgeCorner2 = e->getVoronoiEdge().second.get();
			if(checkEdgePoint(vEdgeCorner1->getPoint(), nextPoint)){
				return e.get();
			}else if(checkEdgePoint(vEdgeCorner2->getPoint(), nextPoint)){
				return e.get();
			}
		}
		return nullptr;
	}
	bool Center::checkEdgePoint(sf::Vector2f corner, sf::Vector2f next){
		if(corner.x >= next.x - 1 && corner.x <= next.x + 1 &&
				corner.y >= next.y - 1 && corner.y <= next.y + 1){
			return true;
		}else{
			return false;
		}
	}
}
