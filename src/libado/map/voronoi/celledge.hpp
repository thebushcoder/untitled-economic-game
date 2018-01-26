/*
 * edge.hpp
 *
 *  Created on: 11 Dec 2017
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_CELLEDGE_HPP_
#define LIBADO_MAP_VORONOI_CELLEDGE_HPP_

#include "cellcorner.hpp"
#include "center.hpp"
#include "../../util/lineShape.hpp"

namespace VoronoiMap{
	typedef std::pair<std::shared_ptr<Center>, std::shared_ptr<Center>> CenterPair;
	typedef std::pair<std::shared_ptr<CellCorner>, std::shared_ptr<CellCorner>> CornerPair;

	class CellEdge{
public:
		~CellEdge(){}

		CenterPair& getDelaunayEdge(){
			return *delaunayEdge;
		}
		sf::Vector2f getMidPoint() const {
			return midPoint;
		}
		CornerPair& getVoronoiEdge(){
			return *voronoiEdge;
		}
		void setDelaunayEdge(CenterPair dEdge){
			delaunayEdge = std::make_shared<CenterPair>(dEdge.first, dEdge.second);
		}
		void setVoronoiEdge(CornerPair vEdge){
			voronoiEdge = std::make_shared<CornerPair>(vEdge.first, vEdge.second);
			calcMidpoint();
		}
		bool operator !=(const CellEdge &b) const{
			return !(*this == b);
		}
		bool operator ==(const CellEdge &b) const;

		bool operator <(const CellEdge& right) const;

		bool isRiver() const {
			return river > 0;
		}
		void setRiver(int river);
		int getRiver(){
			return river;
		}
		sf::LineShape* getDelLine(){
			return delLine.get();
		}
		sf::LineShape* getVorLine(){
			return vorLine.get();
		}
private:
		void calcMidpoint();

		std::shared_ptr<CenterPair> delaunayEdge;	// line connecting polygon center points
		std::shared_ptr<CornerPair> voronoiEdge;	// a polygon outline edge
		std::unique_ptr<sf::LineShape> delLine;
		std::unique_ptr<sf::LineShape> vorLine;
		sf::Vector2f midPoint;
		int river = 0;
	};
}

#endif /* LIBADO_MAP_VORONOI_CELLEDGE_HPP_ */
