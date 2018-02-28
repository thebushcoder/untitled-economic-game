/*
 * kingdomUtil.cpp
 *
 *  Created on: 18 Feb 2018
 *      Author: linbob
 */

#include "kingdomUtil.hpp"
#include "../../test/voronoiScreen.hpp"

namespace KingdomUtil{
	void KingdomUtil::genKingdomArea(std::map<KingdomType, anax::Entity>& kingdomList,
			VoronoiScreen* vorScreen){

		std::unique_ptr<VoronoiMap::VoronoiMap> provVorDia = generateKingdomVoronoi(vorScreen);

		int provCount = provVorDia->getAllCenters().size();

		sf::Vector2f startDwarf(20, 20);
		float dwarfArea = 0.20;
		int dwarfProvs = provCount * dwarfArea;
		sf::Vector2f startElf(20, provVorDia->getMapH() - 20);
		float elfArea = 0.25;
		int elfProvs = provCount * elfArea;
		sf::Vector2f startRepub(provVorDia->getMapW() - 20, 20);
		float repubArea = 0.15;
		int repubProvs = provCount * repubArea;
		sf::Vector2f startHuman(provVorDia->getMapW() - 20, provVorDia->getMapH() - 20);
		float humanArea = 0.40;
		int humanProvs = provCount * humanArea;

		for(auto e : kingdomList){
			ProvincesComponent& p = e.second.getComponent<ProvincesComponent>();
			p.list.clear();
		}

		std::queue<VoronoiMap::Center*> humanFront;
		humanFront.push(provVorDia->getClosestCell(startHuman));
		std::queue<VoronoiMap::Center*> elfFront;
		elfFront.push(provVorDia->getClosestCell(startElf));
		std::queue<VoronoiMap::Center*> dwarfFront;
		dwarfFront.push(provVorDia->getClosestCell(startDwarf));
		std::queue<VoronoiMap::Center*> repubFront;
		repubFront.push(provVorDia->getClosestCell(startRepub));

		do{
			KingdomUtil::floodFill(humanFront, kingdomList[KingdomType::HUMAN],
					provVorDia.get(), humanProvs);
			KingdomUtil::floodFill(elfFront, kingdomList[KingdomType::ELF],
					provVorDia.get(), elfProvs);
			KingdomUtil::floodFill(dwarfFront, kingdomList[KingdomType::DWARF],
					provVorDia.get(), dwarfProvs);
			KingdomUtil::floodFill(repubFront, kingdomList[KingdomType::REPUB],
					provVorDia.get(), repubProvs);
		}while(!humanFront.empty() || !elfFront.empty() ||
				!dwarfFront.empty() || !repubFront.empty());

		int provTotal = 0;
		for(auto e : kingdomList){
			ProvincesComponent& p = e.second.getComponent<ProvincesComponent>();

			provTotal += p.list.size();
		}

		// sometimes flood fill doesnt fill every cell, roughly 1-2. set unfilled cells
		// to their first neighbour's owner/kingdom. change this?
		if(provTotal < provCount){
			for(auto c : provVorDia->getAllCenters()){
				if(c.second->getOwner() == KingdomType::NONE){
					c.second->setOwner(c.second->getNeighbours()[0]->getOwner());
					break;
				}
			}
		}

		for(auto e : kingdomList){
			ProvincesComponent& p = e.second.getComponent<ProvincesComponent>();

			p.list.clear();
		}

		markKingdomArea(kingdomList, provVorDia.get(), vorScreen);

	}

	void KingdomUtil::markKingdomArea(std::map<KingdomType, anax::Entity>& kingdomList,
			VoronoiMap::VoronoiMap* provVorDiam, VoronoiScreen* vorScreen){
		for(auto iterA = vorScreen->getTerrainVorDia()->getAllCenters().begin();
			iterA != vorScreen->getTerrainVorDia()->getAllCenters().end(); ++iterA){
			if(!iterA->second->isOcean()){
				for(auto iterB = provVorDiam->getAllCenters().begin();
						iterB != provVorDiam->getAllCenters().end(); ++iterB){
					std::deque<sf::Vector2f> polyPoints;
					for(int i = 0; i < iterB->second->getLinePoly().getPointCount(); ++i){
						polyPoints.push_back(iterB->second->getLinePoly().getPoint(i));
					}
					if(PolygonUtil::windingAlgo(iterA->second->getPoint(), polyPoints)){
						ProvincesComponent& p = kingdomList[iterB->second->getOwner()].
								getComponent<ProvincesComponent>();
						p.list.push_back(iterA->second.get());
						iterA->second->setOwner(iterB->second->getOwner());
					}
				}
			}
		}
	}

	std::unique_ptr<VoronoiMap::VoronoiMap> KingdomUtil::generateKingdomVoronoi(VoronoiScreen* vorScreen){
		std::vector<VoronoiMap::CellEdge*> landOutline;

		// get edges with ocean-land borders
		for(auto iter = vorScreen->getTerrainVorDia()->getAllCenters().begin();
				iter != vorScreen->getTerrainVorDia()->getAllCenters().end(); ++iter){
			if(!iter->second->isOcean()){
				for(auto e : iter->second->getPolyEdges()){
					if(!e->getDelaunayEdge()->hasPointA() || !e->getDelaunayEdge()->hasPointB()) continue;
					if(e->getDelaunayEdge()->getPointA()->isOcean() !=
							e->getDelaunayEdge()->getPointB()->isOcean()){
						landOutline.push_back(e.get());
					}
				}
			}
		}

		sf::ConvexShape* landPoly = new sf::ConvexShape(landOutline.size());
		landPoly->setFillColor(sf::Color::Magenta);

		// choose edge to start walking from
		VoronoiMap::CellEdge* c = landOutline[0];
		sf::Vector2f current = c->getVoronoiEdge()->getPointA()->getPoint();
		sf::Vector2f nextPoint = c->getVoronoiEdge()->getPointB()->getPoint();

		std::vector<VoronoiMap::CellEdge*> visited;
		std::deque<sf::Vector2f> orderedOutline;

		// walk land outline edges to form a polygon
		for(int i = 0; i < landOutline.size(); ++i){
			if(!c) break;
			if(i > 0){
				// determine which point is the next point
				if(VoronoiMap::Center::checkEdgePoint(
						c->getVoronoiEdge()->getPointA()->getPoint(), nextPoint)){
					current = c->getVoronoiEdge()->getPointA()->getPoint();
					nextPoint = c->getVoronoiEdge()->getPointB()->getPoint();
				}else{
					current = c->getVoronoiEdge()->getPointB()->getPoint();
					nextPoint = c->getVoronoiEdge()->getPointA()->getPoint();
				}
			}

			landPoly->setPoint(i, current);
			visited.push_back(c);
			orderedOutline.push_back(current);

			bool flag = false;
			// find next connected edge to walk
			for(auto e : landOutline){
				if(std::find(visited.begin(), visited.end(), e) != visited.end()) continue;

				VoronoiMap::CellCorner* vEdgeCorner1 = e->getVoronoiEdge()->getPointA().get();
				VoronoiMap::CellCorner* vEdgeCorner2 = e->getVoronoiEdge()->getPointB().get();

				if(VoronoiMap::Center::checkEdgePoint(vEdgeCorner1->getPoint(), nextPoint)){
					c = e;
					flag = true;
					break;
				}else if(VoronoiMap::Center::checkEdgePoint(vEdgeCorner2->getPoint(), nextPoint)){
					c = e;
					flag = true;
					break;
				}
			}
			if(!flag){
				c = nullptr;
			}
		}

		return std::unique_ptr<VoronoiMap::VoronoiMap>(
				new VoronoiMap::VoronoiMap(200, vorScreen->getTerrainVorDia()->getMapW(),
						vorScreen->getTerrainVorDia()->getMapH(), orderedOutline));

//		vorScreen->getProvinceVorDia()->setLandPoly(std::unique_ptr<sf::ConvexShape>(std::move(landPoly)));
	}

	void KingdomUtil::placeSettlements(VoronoiScreen* vorScreen){
		// mark best tile
		// loop tiles in a kingdoms ProvenceComponent's list
		// calc weight for tile
		// if current tile's weight > best tile, best tile == current

		// ocean tile has 1-2(3?) ocean neighbours
		// ocean tile has no null delaunay


	}

	void KingdomUtil::floodFill(std::queue<VoronoiMap::Center*>& front, anax::Entity e,
			VoronoiMap::VoronoiMap* map, int factionProvs){

		if(front.empty()) return;

		ProvincesComponent& p = e.getComponent<ProvincesComponent>();
		KingdomComponent& k = e.getComponent<KingdomComponent>();

		if(p.list.size() >= factionProvs){
			for(int i = 0; i < front.size(); ++i){
				front.pop();
			}
			return;
		}

		VoronoiMap::Center* current = front.front();
		front.pop();

		for(int i = 0; i < current->getNeighbours().size(); ++i){
			VoronoiMap::Center* n = current->getNeighbours()[i].get();
			if(n->getOwner() == KingdomType::NONE &&
					std::find(p.list.begin(), p.list.end(),	n) == p.list.end() &&
					p.list.size() < factionProvs){
				front.push(n);
				p.list.push_back(n);
				n->setOwner(k.getType());
			}
		}
	}

}
