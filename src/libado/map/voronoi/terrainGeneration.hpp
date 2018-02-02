/*
 * terrainGeneration.hpp
 *
 *  Created on: 1 Feb 2018
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_TERRAINGENERATION_HPP_
#define LIBADO_MAP_VORONOI_TERRAINGENERATION_HPP_

#include "voronoiMap.hpp"

namespace VoronoiMap{
	struct TerrainGeneration{
		static void generateWater(VoronoiMap* map){
			// center is outside island shape - mark as water
			for(auto iter = map->getCorners().begin(); iter != map->getCorners().end(); ++iter){
				if(!map->insideLandShape(iter->second->getPoint())){
					iter->second->setIsWater(true);
				}
				if(iter->second->isMapBorder()){
					iter->second->setElevation(0.0);
				}
			}

			// create water centers - additional ocean + lakes
			int numWater;
			float lakeThreshold = 0.8;
			std::queue<Center*> front;

			for(auto iter = map->getAllCenters().begin(); iter != map->getAllCenters().end(); ++iter){
				numWater = 0;
				for(auto c : iter->second->getCorners()){
					// mark border + ocean
					if(c->isMapBorder()){
						iter->second->setIsOcean(true);
						iter->second->setIsBorder(true);
						c->setIsWater(true);
						front.push(iter->second.get());
					}
					if(c->isWater()){
						numWater += 1;
					}
				}
				// mark lakes
				bool flag = (iter->second->isWater() ||
						numWater >= iter->second->getCorners().size() * lakeThreshold);
				iter->second->setIsWater(flag);

	//			iter->second->setRainfall();
			}

			while(!front.empty()){
				Center* current = front.front();
				front.pop();

				for(auto n : current->getNeighbours()){
					if(n->isWater() && !n->isOcean()){
						n->setIsOcean(true);
						front.push(n.get());
					}
				}
			}

			// mark coastline
			for(auto iter = map->getAllCenters().begin(); iter != map->getAllCenters().end(); ++iter){
				int numOcean = 0;
				int numLand = 0;

				for(auto n : iter->second->getNeighbours()){
					numOcean += (n->isOcean() ? 1 : 0);
					numLand += (!n->isWater() ? 1 : 0);
				}
				bool flag = (numOcean > 0) && (numLand > 0);
				iter->second->setIsCoast(flag);
			}

			for(auto iter = map->getCorners().begin(); iter != map->getCorners().end(); ++iter){
				int numOcean = 0;
				int numLand = 0;
				for(auto t : iter->second->getTouches()){
					numOcean += (t->isOcean() ? 1 : 0);
					numLand += (!t->isWater() ? 1 : 0);
				}
				bool flag = (numOcean >= iter->second->getTouches().size());
				iter->second->setIsOcean(flag);

				flag = (numOcean > 0) && (numLand > 0);
				iter->second->setIsCoast(flag);

				flag = iter->second->isMapBorder() ||
						((numLand != iter->second->getTouches().size()) &&
								!iter->second->isCoast());
				iter->second->setIsWater(flag);
			}
		}
		static void generateElevation(VoronoiMap* map){
			// 	use perlin noise to generate height map
			FastNoise perlinNoise;
			perlinNoise.SetSeed(map->getRandGen()());
			perlinNoise.SetNoiseType(FastNoise::Perlin);
			perlinNoise.SetInterp(FastNoise::Linear);
			// decrease to create larger patches of similar height
			// increase for more extreme, rugged patches of mountains
			perlinNoise.SetFrequency(0.0070);

			//	set poly/center elevation
			for(auto iter = map->getAllCenters().begin(); iter != map->getAllCenters().end(); ++iter){
				if(iter->second->isWater()){
					iter->second->setElevation(0.0);
				}else{
					// elevFactor - increase to increase general elevation,
					// decrease to produce flatter land
					float elevFactor = 0.9;
					float e = (elevFactor + perlinNoise.GetNoise(iter->second->getPoint().x,
							iter->second->getPoint().y)) / 2;
					iter->second->setElevation(e);
				}
			}
			//	set corner elevations to the avg of neighbouring poly/centers
			for(auto iter = map->getCorners().begin(); iter != map->getCorners().end(); ++iter){
				float e = 0;
				for(auto c : iter->second->getTouches()){
					e += c->getElevation();
				}
				e = e / iter->second->getTouches().size();
				iter->second->setElevation(e);
			}

			// fill sinks - erodes terrain to make rivers look more natural
			// Ref: (function fillSinks(h, epsilon)) https://github.com/mewo2/terrain/blob/master/terrain.js
			/*
			 *  There's an obvious problem when we reach gridpoints which are lower than all
			 *  of their neighbours. Do we route the water back uphill? This will probably
			 *  lead to cycles in the water system, which are trouble. Instead, we want to
			 *  fill in these gaps (often called sinks or depressions), so that the water
			 *  always runs downhill all the way to the edge.
			 */
			std::map<std::pair<float, float>, float> newHeightmap;
			std::queue<CellCorner*> front;

			for(auto iter = map->getCorners().begin(); iter != map->getCorners().end(); ++iter){
				if(iter->second->isMapBorder()){
					newHeightmap[{iter->second->getPoint().x, iter->second->getPoint().y}]
								 = 0;
					front.push(iter->second.get());
				}else{
					newHeightmap[{iter->second->getPoint().x, iter->second->getPoint().y}]
								 = 9999;
				}
			}

			float epsilon = 0.0125;
			while(!front.empty()){
				CellCorner* c = front.front();
				front.pop();

				for(auto n : c->getAdjacent()){
					if(c->getElevation() >=
							newHeightmap[{n->getPoint().x, n->getPoint().y}] + epsilon){
						newHeightmap[{c->getPoint().x, c->getPoint().y}] = c->getElevation();
						front.push(n.get());
						break;
					}
					float k = newHeightmap[{n->getPoint().x, n->getPoint().y}] + epsilon;
					if((newHeightmap[{n->getPoint().x, n->getPoint().y}] > k) &&
							(k > c->getElevation())){
						newHeightmap[{c->getPoint().x, c->getPoint().y}] = k;
						front.push(n.get());
					}
				}
			}

			std::map<std::pair<float, float>, std::shared_ptr<Center>> list;

			// create list of non-water tiles
			for(auto iter = map->getAllCenters().begin(); iter != map->getAllCenters().end(); ++iter){
				if(iter->second->getElevation() <= 0.15){
					iter->second->setIsWater(true);
					for(auto c : iter->second->getCorners()){
						c->setIsWater(true);
					}
				}else if(!iter->second->isWater() && iter->second->getElevation() > 0.15){
					list[{iter->second->getPoint().x,
						iter->second->getPoint().y}] = iter->second;
				}
			}
			map->setLandCenters(list);
		}
		static void generateRivers(VoronoiMap* map){
			// river generation
			/*
			 * 	delaunay triangle is a voronoi corner point surrounded by a triangle made
			 * 	up from voronoi center points. Corner point contains links to delaunay edges/ centers
			 */
			std::vector<CellCorner*> delTriFront;

			std::map<CellCorner*, CellCorner*> outflow;		// del tri paths
			std::map<CellCorner*, CellCorner*> watershed;	// del tri

			// Establish corner queue front - uses coastline
			for(auto iter = map->getLandCenters().begin(); iter != map->getLandCenters().end(); ++iter){
				if(iter->second->isCoast()){
					for(auto c : iter->second->getCorners()){
						if(c->isCoast()){
							delTriFront.push_back(c.get());
							// init delaunay maps
							watershed[c.get()] = c.get();
							outflow[c.get()] = c.get();
						}
					}
				}
			}

			// walk the corner front/queue
			for(int i = 0; i < delTriFront.size(); ++i){

				// elevation sorted rotated list
				int pivot = std::ceil((delTriFront.size() - i) / 4);
				pivot = pivot > 0 ? pivot : 1;	// workaround to prevent infinite loop

				for(int k = delTriFront.size() - 1; k > i; k -= pivot){
					float a = delTriFront[k]->getElevation();
					float b = delTriFront[i]->getElevation();

					if(a < b){
						CellCorner* s = delTriFront[k];
						delTriFront[k] = delTriFront[i];
						delTriFront[i] = s;
					}
				}

				// mark water outflow and add outflow point to queue
				CellCorner* c = delTriFront[i];
				for(auto n : c->getAdjacent()){
					if(outflow.find(n.get()) == outflow.end() && (!n->isWater() && !n->isOcean())){
						outflow[n.get()] = c;
						watershed[n.get()] = watershed[c];
						delTriFront.push_back(n.get());
					}
				}
			}

			// establish river size/width - using the Strahler number
			std::map<CellCorner*, int> riverSize;

			for(int i = delTriFront.size() - 1; i >= 0; --i){
				CellCorner* upStream = delTriFront[i];
				CellCorner* downStream = outflow[upStream];

				if(!downStream) continue;

				if(riverSize.find(upStream) == riverSize.end()){
					riverSize[upStream] = 1;
				}

				if(riverSize[upStream] == riverSize[downStream]){
					riverSize[downStream]++;
				}else if(riverSize[upStream] > riverSize[downStream]){
					riverSize[downStream] = riverSize[upStream];
				}
			}

			// 2 pass loop - required to tidy up stray coastal rivers
			for(int i = 0; i < 2; ++i){
				// cull excess rivers - only interested in wide rivers and those attached to them
				for(auto iter = riverSize.begin(); iter != riverSize.end(); ++iter){
					if(iter->first->isCoast()){
						iter->second = 0;
					}

					if(iter->second > 1) continue;

					bool hasWideNeighbour = false;
					for(auto n : iter->first->getAdjacent()){
						if(riverSize[n.get()] > 1){
							hasWideNeighbour = true;
						}
					}
					if(!hasWideNeighbour){
						iter->second = 0;
					}
				}
			}

			// mark voronoi edges as rivers
			for(int i = 0; i < map->getCorners().size(); ++i){
				CellCorner* c = delTriFront[i];
				int rSize = riverSize[c];

				if(rSize <= 0) continue;

				CellCorner* downStream = outflow[c];

				if(downStream && !c->isWater() && !downStream->isWater()){
					CellEdge* e = c->getEdge(downStream);
					e->setRiver(rSize);
				}
			}
		}

		static void generateMoisture(VoronoiMap* map){
			FastNoise perlinNoise;
			perlinNoise.SetSeed(map->getRandGen()());
			perlinNoise.SetNoiseType(FastNoise::Perlin);
			perlinNoise.SetInterp(FastNoise::Linear);
			perlinNoise.SetFrequency(0.0100);

			for(auto iter = map->getAllCenters().begin(); iter != map->getAllCenters().end(); ++iter){
				// lakes and ocean tiles are max moisture
				if(iter->second->isWater()){
					iter->second->setMoisture(1);
					continue;
				}

				float moist = std::abs(perlinNoise.GetNoise(iter->second->getPoint().x,
											iter->second->getPoint().y));

				iter->second->nMoist = moist;
				printf("=======================\n");
				printf("Perlin moisture : %f\n", moist);

				// moisture falls off with elevation (higher rainfall with higher elev)
				// 0.7 is snowy mountain peak. need to redo elev-moisture balance

				float elevMoist = 0;
				if(iter->second->getElevation() >= 0.9){
					elevMoist += 0.8;
					iter->second->eMoist = 0.8;
				}else if(iter->second->getElevation() >= 0.7){
					elevMoist += 0.7;
					iter->second->eMoist = 0.7;
				}else if(iter->second->getElevation() >= 0.6){
					elevMoist += 0.5;
					iter->second->eMoist = 0.5;
				}else if(iter->second->getElevation() >= 0.5){
					elevMoist += 0.4;
					iter->second->eMoist = 0.4;
				}

				if(elevMoist > moist){
					moist = elevMoist;
				}

				float waterM = 0;
				// check neighbour tiles to a depth of 2 for water tiles
				bool nearWater = false;
				for(auto n : iter->second->getNeighbours()){
					nearWater = false;
					if(n->isWater()){
						if(n->isOcean()){
							waterM = waterM > 0.3 ? waterM : 0.3;
							nearWater = true;
							continue;
						}
						waterM = waterM > 0.2 ? waterM : 0.2;
						nearWater = true;
						continue;
					}
					for(auto d : n->getNeighbours()){
						if(d->isWater()){
							if(d->isOcean()){
								waterM = waterM > 0.15 ? waterM : 0.15;
								nearWater = true;
								continue;
							}
							waterM = waterM > 0.08 ? waterM : 0.08;
							nearWater = true;
							continue;
						}
					}
				}

				// if no neighbours are water tiles, check for rivers bordering the tile
				if(!nearWater){
					for(auto e : iter->second->getPolyEdges()){
						if(e->isRiver()){
							waterM += e->getRiver() * 0.05;
						}
					}
				}

				moist += waterM;
				iter->second->wMoist = waterM;
				iter->second->setMoisture(moist);
			}

		}
	};
}


#endif /* LIBADO_MAP_VORONOI_TERRAINGENERATION_HPP_ */