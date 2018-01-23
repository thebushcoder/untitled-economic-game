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
}
