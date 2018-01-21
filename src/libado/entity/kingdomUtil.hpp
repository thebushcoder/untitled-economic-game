/*
 * kingdomUtil.hpp
 *
 *  Created on: 24 Dec 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_KINGDOMUTIL_HPP_
#define LIBADO_ENTITY_KINGDOMUTIL_HPP_

#include <anax/Entity.hpp>
#include <queue>

#include "kingdomType.hpp"
#include "../map/voronoi/voronoiMap.hpp"
#include "../map/voronoi/center.hpp"
#include "components/kingdomComponent.hpp"
#include "components/provincesComponent.hpp"

namespace KingdomUtil{
	struct KingdomUtil{

		static void genKingdomArea(std::map<KingdomType, anax::Entity>& kingdomList,
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
				floodFill(humanFront, kingdomList[KingdomType::HUMAN], map, humanProvs);
				floodFill(elfFront, kingdomList[KingdomType::ELF], map, elfProvs);
				floodFill(dwarfFront, kingdomList[KingdomType::DWARF], map, dwarfProvs);
				floodFill(repubFront, kingdomList[KingdomType::REPUB], map, repubProvs);
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

		static void floodFill(std::queue<VoronoiMap::Center*>& front, anax::Entity e,
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
	};
}

#endif /* LIBADO_ENTITY_KINGDOMUTIL_HPP_ */
