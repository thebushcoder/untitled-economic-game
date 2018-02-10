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
	template <class T> class LineEdge{
	public:
		std::shared_ptr<T>& getPointA(){
			return pointA;
		}
		void setPointA(std::shared_ptr<T> pointA) {
			this->pointA = pointA;
			aIsSet = true;
		}
		std::shared_ptr<T>& getPointB(){
			return pointB;
		}
		void setPointB(std::shared_ptr<T> pointB) {
			this->pointB = pointB;
			bIsSet = true;
		}
		bool hasPointA(){
			return aIsSet;
		}
		bool hasPointB(){
			return bIsSet;
		}
	private:
		bool aIsSet = false;
		bool bIsSet = false;
		std::shared_ptr<T> pointA = nullptr;
		std::shared_ptr<T> pointB = nullptr;
	};

	class CellEdge{
public:
		~CellEdge(){}

		LineEdge<Center>* getDelaunayEdge(){
			return delaunayEdge.get();
		}
		sf::Vector2f getMidPoint() const {
			return midPoint;
		}
		LineEdge<CellCorner>* getVoronoiEdge(){
			return voronoiEdge.get();
		}
		void setDelaunayEdge(std::shared_ptr<LineEdge<Center>> dEdge){
			delaunayEdge = dEdge;
		}
		void setVoronoiEdge(std::shared_ptr<LineEdge<CellCorner>> vEdge){
			voronoiEdge = vEdge;
			calcMidpoint();
		}
		bool operator !=(const CellEdge &b) const{
			return !(*this == b);
		}
		bool operator ==(const CellEdge &b) const;

		bool operator <(const CellEdge& right) const;

		bool isRiver(){
			return riverSize > 0;
		}
		void setRiver(int river);
		int getRiver(){
			return riverSize;
		}
		sf::LineShape* getDelLine(){
			return delLine.get();
		}
		sf::LineShape* getVorLine(){
			return vorLine.get();
		}
		bool hasCorner(CellCorner* c){
			return (voronoiEdge->getPointA().get() == c) || (voronoiEdge->getPointB().get() == c);
		}
private:
		void calcMidpoint();

		std::shared_ptr<LineEdge<Center>> delaunayEdge;	// line connecting polygon center points
		std::shared_ptr<LineEdge<CellCorner>> voronoiEdge;	// a polygon outline edge
		std::unique_ptr<sf::LineShape> delLine;
		std::unique_ptr<sf::LineShape> vorLine;
		sf::Vector2f midPoint;
		int riverSize = 0;
	};
}

#endif /* LIBADO_MAP_VORONOI_CELLEDGE_HPP_ */
