/*
 * noisyEdges.hpp
 *
 *  Created on: 10 Feb 2018
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_NOISYEDGES_HPP_
#define LIBADO_MAP_VORONOI_NOISYEDGES_HPP_

#include <SFML/Graphics.hpp>

#include "voronoiMap.hpp"

namespace VoronoiMap{
	struct NoisyEdges{
		void generateNoisyEdges(VoronoiMap* map){
			for(auto e : map->getEdges()){
				if(!e->getDelaunayEdge()->getPointA() || !e->getDelaunayEdge()->getPointB()
						|| (e->getVoronoiEdge()->getPointA()->isMapBorder() &&
						e->getVoronoiEdge()->getPointB()->isMapBorder())) continue;

				if(calcDist(e->getVoronoiEdge()->getPointA()->getPoint(),
						e->getVoronoiEdge()->getPointB()->getPoint()) <= 11.0) continue;

				list = std::vector<std::pair<sf::Vector2f, float>>();
				current = e.get();

				subDivideQuad(e->getVoronoiEdge()->getPointA()->getPoint(),
						e->getVoronoiEdge()->getPointB()->getPoint(),
						e->getDelaunayEdge()->getPointA()->getPoint(),
						e->getDelaunayEdge()->getPointB()->getPoint(),
						subDivideNum, map);

				// sort points by dist from Voronoi point A
				std::sort(list.begin(), list.end(), [](const std::pair<sf::Vector2f, float>& a,
						const std::pair<sf::Vector2f, float>& b){
					return (a.second < b.second);
				});

				sf::Vector2f c = e->getVoronoiEdge()->getPointA()->getPoint();
				for(auto p : list){
					e->setNoisyLine(std::unique_ptr<sf::LineShape>(new sf::LineShape(c, p.first)));
					c = p.first;
				}
				e->setNoisyLine(std::unique_ptr<sf::LineShape>(new sf::LineShape(c,
						e->getVoronoiEdge()->getPointB()->getPoint())));

			}
		}
		void generateNoisyPolys(VoronoiMap* map){
			printf("&&&&&&&&&&&&&&&&&&&&&&&\n");
			for(auto iter = map->getAllCenters().begin(); iter != map->getAllCenters().end(); ++iter){
				int s = 0;
				for(auto e : iter->second->getPolyEdges()){
					float pts = 0;

					printf("Edge noisy line count : %d\n", e->getNoisyLine().size());

					if(e->getNoisyLine().size() > 0){
						pts = e->getNoisyLine().size();
					}else{
						pts = 1;
					}
					s += pts;
				}

				printf("\tEdge poly point count : %d\n", s);

				iter->second->resetPolyShape();

				iter->second->getPolyShape().setPointCount(s);

				CellEdge* e = iter->second->getPolyEdges()[0].get();
				sf::Vector2f current = e->getVoronoiEdge()->getPointA()->getPoint();
				sf::Vector2f nextPoint = e->getVoronoiEdge()->getPointB()->getPoint();
				std::vector<CellEdge*> visited;

				int count = 0;
				// walk corner points and construct polygon
				for(int i = 0; i < s; ++i){
					if(!e)	break;

					if(i > 0){
						if(Center::checkEdgePoint(e->getVoronoiEdge()->getPointA()->getPoint(), nextPoint)){
							current = e->getVoronoiEdge()->getPointA()->getPoint();
							nextPoint = e->getVoronoiEdge()->getPointB()->getPoint();
						}else{
							current = e->getVoronoiEdge()->getPointB()->getPoint();
							nextPoint = e->getVoronoiEdge()->getPointA()->getPoint();
						}
					}

					if(e->getNoisyLine().size() > 0){

						int lineX = std::floor(e->getNoisyLine()[0]->getPointA().x);
						int lineY = std::floor(e->getNoisyLine()[0]->getPointA().y);
						int currentX = std::floor(current.x);
						int currentY = std::floor(current.y);

						if(lineX == currentX && lineY == currentY){

							for(int x = 0; x < e->getNoisyLine().size(); ++x){
								iter->second->getPolyShape().setPoint(count++,
										e->getNoisyLine()[x]->getPointA());
							}
						}else{

							for(int x = e->getNoisyLine().size() - 1; x >= 0; --x){
								iter->second->getPolyShape().setPoint(count++,
										e->getNoisyLine()[x]->getPointB());
							}
						}
					}else{
						iter->second->getPolyShape().setPoint(count++, current);
					}

					visited.push_back(e);
					e = iter->second->getNextVorEdge(&visited, nextPoint);
				}
			}
		}
		static NoisyEdges* getInstance(){
			static std::unique_ptr<NoisyEdges> instance;
			if(!instance){
				instance = std::unique_ptr<NoisyEdges>(new NoisyEdges());
			}
			return instance.get();
		}
	private:
		float calcDist(sf::Vector2f a, sf::Vector2f b){
			return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
		}
		void subDivideQuad(sf::Vector2f vorA, sf::Vector2f vorB,
				sf::Vector2f delA, sf::Vector2f delB, int count, VoronoiMap* map){

			sf::Vector2f mid = calcMidPoint(delA, delB, map);
			list.push_back({mid, calcDist(mid,
					current->getVoronoiEdge()->getPointA()->getPoint())});

			if(count <= 1) return;

			int nextCount = count / 2;

			// divide left
			sf::Vector2f delMidA = sf::Vector2f((delA.x + vorA.x) / 2,
					(delA.y + vorA.y) / 2);
			sf::Vector2f delMidB = sf::Vector2f((delB.x + vorA.x) / 2,
					(delB.y + vorA.y) / 2);
			subDivideQuad(vorA, mid, delMidA, delMidB, nextCount, map);

			// divide right
			delMidA = sf::Vector2f((delA.x + vorB.x) / 2,
					(delA.y + vorB.y) / 2);
			delMidB = sf::Vector2f((delB.x + vorB.x) / 2,
					(delB.y + vorB.y) / 2);
			subDivideQuad(mid, vorB, delMidA, delMidB, nextCount, map);
		}

		sf::Vector2f calcMidPoint(sf::Vector2f delA, sf::Vector2f delB, VoronoiMap* map){
			// amp = +- range from midpoint, choose rand point in range
			std::uniform_real_distribution<> ampDis(-0.40, 0.40);

			float midX = (delA.x +	delB.x) / 2;
			float midY = (delA.y +	delB.y) / 2;

			float amp = ampDis(map->getRandGen());
			sf::Vector2f mid(midX, midY);

			// calc modpoint offset
			if(amp > 0.0){
				float dX = std::max(delA.x, mid.x) - std::min(delA.x, mid.x);
				float dY = std::max(delA.y, mid.y) - std::min(delA.y, mid.y);
				midX += dX * amp;
				midY += dY * amp;
			}else if(amp < 0.0){
				float dX = std::max(delB.x, mid.x) - std::min(delB.x, mid.x);
				float dY = std::max(delB.y, mid.y) - std::min(delB.y, mid.y);
				midX += dX * amp;
				midY += dY * amp;
			}
			mid.x = midX;
			mid.y = midY;

			return mid;
		}

		static const int subDivideNum = std::pow(2, 2);	//2^2 = 4

		std::vector<std::pair<sf::Vector2f, float>> list;
		CellEdge* current;
	};
}

#endif /* LIBADO_MAP_VORONOI_NOISYEDGES_HPP_ */
