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
			VoronoiMap::VoronoiMap* map){
		int provCount = map->getAllCenters().size();

		sf::Vector2f startDwarf(20, 20);
		float dwarfArea = 0.20;
		int dwarfProvs = provCount * dwarfArea;
		sf::Vector2f startElf(20, map->getMapH() - 20);
		float elfArea = 0.25;
		int elfProvs = provCount * elfArea;
		sf::Vector2f startRepub(map->getMapW() - 20, 20);
		float repubArea = 0.15;
		int repubProvs = provCount * repubArea;
		sf::Vector2f startHuman(map->getMapW() - 20, map->getMapH() - 20);
		float humanArea = 0.40;
		int humanProvs = provCount * humanArea;

		for(auto e : kingdomList){
			ProvincesComponent& p = e.second.getComponent<ProvincesComponent>();
			p.list.clear();
		}

		printf("Dwarf kingdom size: %d\n", dwarfProvs);
		printf("Elf kingdom size: %d\n", elfProvs);
		printf("Repub kingdom size: %d\n", repubProvs);
		printf("Human kingdom size: %d\n\n", humanProvs);

		std::queue<VoronoiMap::Center*> humanFront;
		humanFront.push(map->getClosestCell(startHuman));
		std::queue<VoronoiMap::Center*> elfFront;
		elfFront.push(map->getClosestCell(startElf));
		std::queue<VoronoiMap::Center*> dwarfFront;
		dwarfFront.push(map->getClosestCell(startDwarf));
		std::queue<VoronoiMap::Center*> repubFront;
		repubFront.push(map->getClosestCell(startRepub));

		do{
			KingdomUtil::floodFill(humanFront, kingdomList[KingdomType::HUMAN], map, humanProvs);
			KingdomUtil::floodFill(elfFront, kingdomList[KingdomType::ELF], map, elfProvs);
			KingdomUtil::floodFill(dwarfFront, kingdomList[KingdomType::DWARF], map, dwarfProvs);
			KingdomUtil::floodFill(repubFront, kingdomList[KingdomType::REPUB], map, repubProvs);
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
			for(auto c : map->getAllCenters()){
				if(c.second->getOwner() == KingdomType::NONE){
					c.second->setOwner(c.second->getNeighbours()[0]->getOwner());
					break;
				}
			}
		}

		for(auto e : kingdomList){
			ProvincesComponent& p = e.second.getComponent<ProvincesComponent>();
			KingdomComponent& k = e.second.getComponent<KingdomComponent>();

			printf("%s province number: %d\n", TypetoString(k.getType()).c_str(),
					p.list.size());
		}
	}

	void KingdomUtil::generateKingdomVoronoi(VoronoiScreen* vorScreen){
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

		vorScreen->setProvinceVorDia(std::unique_ptr<VoronoiMap::VoronoiMap>(
				new VoronoiMap::VoronoiMap(200, vorScreen->getTerrainVorDia()->getMapW(),
						vorScreen->getTerrainVorDia()->getMapH(), orderedOutline)));

//		vorScreen->getProvinceVorDia()->setLandPoly(std::unique_ptr<sf::ConvexShape>(std::move(landPoly)));
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
