/*
 * center.cpp
 *
 *  Created on: 13 Dec 2017
 *      Author: linbob
 */

#include "center.hpp"
#include "celledge.hpp"

namespace VoronoiMap{
	// walks(clockwise/counter) the voronoi(polygon) edges and returns the next unvisited
	// edge connected to nextPoint
	CellEdge* Center::getNextVorEdge(std::vector<CellEdge*>* visited, sf::Vector2f nextPoint){
		for(auto e : polyEdges){
			if(std::find(visited->begin(), visited->end(), e.get()) != visited->end()) continue;

			CellCorner* vEdgeCorner1 = e->getVoronoiEdge()->getPointA().get();
			CellCorner* vEdgeCorner2 = e->getVoronoiEdge()->getPointB().get();
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
	CellEdge* Center::getDelEdge(Center* neighbour){
		for(auto e : polyEdges){
			if((e->getDelaunayEdge()->getPointA().get() == this ||
					e->getDelaunayEdge()->getPointB().get() == this) &&
					(e->getDelaunayEdge()->getPointA().get() == neighbour ||
							e->getDelaunayEdge()->getPointB().get() == neighbour)){
				return e.get();
			}
		}
		return nullptr;
	}
}
