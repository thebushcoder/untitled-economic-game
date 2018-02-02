/*
 * cellcorner.cpp
 *
 *  Created on: 22 Jan 2018
 *      Author: linbob
 */

#include "cellcorner.hpp"

namespace VoronoiMap{
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
