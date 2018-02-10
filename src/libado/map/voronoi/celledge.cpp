/*
 * edge.cpp
 *
 *  Created on: 12 Dec 2017
 *      Author: linbob
 */

#include "celledge.hpp"

namespace VoronoiMap{
	void CellEdge::calcMidpoint(){
		float x = (voronoiEdge->getPointA()->getPoint().x +
				voronoiEdge->getPointB()->getPoint().x) / 2;
		float y = (voronoiEdge->getPointA()->getPoint().y +
				voronoiEdge->getPointB()->getPoint().y) / 2;
		midPoint = sf::Vector2f(x, y);
	}
	bool CellEdge::operator ==(const CellEdge &b) const{
		sf::Vector2f lV1 = this->voronoiEdge->getPointA()->getPoint();
		sf::Vector2f lV2 = this->voronoiEdge->getPointB()->getPoint();
		sf::Vector2f rV1 = b.voronoiEdge->getPointA()->getPoint();
		sf::Vector2f rV2 = b.voronoiEdge->getPointB()->getPoint();
		return (lV1 == rV1) && (lV2 == rV2);
	}
	bool CellEdge::operator <(const CellEdge& right) const{
		sf::Vector2f lV1 = this->voronoiEdge->getPointA()->getPoint();
		sf::Vector2f lV2 = this->voronoiEdge->getPointB()->getPoint();
		sf::Vector2f rV1 = right.voronoiEdge->getPointA()->getPoint();
		sf::Vector2f rV2 = right.voronoiEdge->getPointB()->getPoint();
		return ((lV1.x < rV1.x) && (lV1.y < rV1.y)) && ((lV2.x < rV2.x) && (lV2.y < rV2.y));
	}
	void CellEdge::setRiver(int river) {
		if(river > 0){
			vorLine = std::unique_ptr<sf::LineShape>(
					new sf::LineShape(voronoiEdge->getPointA()->getPoint(),
					voronoiEdge->getPointB()->getPoint())
			);
			vorLine->setThickness(
					river > 4 ? 3 : river
			);
			vorLine->setFillColor(sf::Color::Blue);
		}
		this->riverSize = river;
	}
}
