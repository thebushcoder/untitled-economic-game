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
			if((e->getVoronoiEdge()->getPointA().get() == this ||
					e->getVoronoiEdge()->getPointB().get() == this) &&
					(e->getVoronoiEdge()->getPointA().get() == n ||
							e->getVoronoiEdge()->getPointB().get() == n)){
				return e.get();
			}
		}
		return nullptr;
	}
	std::shared_ptr<River>& CellCorner::getRiver(){
		return river;
	}
	void CellCorner::setRiver(std::shared_ptr<River> river) {
		this->river = river;
	}
}
