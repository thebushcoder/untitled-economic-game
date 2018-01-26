/*
 * cellcorner.cpp
 *
 *  Created on: 22 Jan 2018
 *      Author: linbob
 */

#include "cellcorner.hpp"

namespace VoronoiMap{
	CellEdge* CellCorner::getDownslopeEdge(){
		for(auto e : protrudes){
			if(e->getVoronoiEdge().first.get() == downSlope.get() ||
					e->getVoronoiEdge().second.get() == downSlope.get()){
				return e.get();
			}
		}
		return nullptr;
	}
	CellEdge* CellCorner::getEdge(CellCorner* n){
		for(auto e : protrudes){
			if((e->getVoronoiEdge().first.get() == this ||
					e->getVoronoiEdge().second.get() == this) &&
					(e->getVoronoiEdge().first.get() == n ||
							e->getVoronoiEdge().second.get() == n)){
				return e.get();
			}
		}
		return nullptr;
	}
}
