/*
 * river.hpp
 *
 *  Created on: 5 Feb 2018
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_RIVER_HPP_
#define LIBADO_MAP_VORONOI_RIVER_HPP_

#include <memory>

//#include "cellcorner.hpp"
#include "celledge.hpp"

class CellCorner;
namespace VoronoiMap{
	class River{
	public:
		std::vector<std::shared_ptr<CellCorner>>& getRiverCorners(){
			return riverCorners;
		}

		std::vector<std::shared_ptr<CellEdge>>& getRiverEdges(){
			return riverEdges;
		}
		std::vector<std::shared_ptr<CellCorner>> getCornerPath(CellCorner* src, CellCorner* dest);
		bool isUpstream(CellCorner* src, CellCorner* dest);
		bool isDownstream(CellCorner* src, CellCorner* dest);

		std::shared_ptr<CellCorner>& getRiverEnd(){
			return riverEnd;
		}
		void setRiverEnd(std::shared_ptr<CellCorner> riverEnd){
			this->riverEnd = riverEnd;
		}

	private:
		std::shared_ptr<CellCorner> riverEnd;
		std::vector<std::shared_ptr<CellCorner>> riverCorners;
		std::vector<std::shared_ptr<CellEdge>> riverEdges;
	};
}

#endif /* LIBADO_MAP_VORONOI_RIVER_HPP_ */
