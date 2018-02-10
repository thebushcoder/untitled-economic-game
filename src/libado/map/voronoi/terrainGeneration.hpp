/*
 * terrainGeneration.hpp
 *
 *  Created on: 1 Feb 2018
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_TERRAINGENERATION_HPP_
#define LIBADO_MAP_VORONOI_TERRAINGENERATION_HPP_

#include "voronoiMap.hpp"
#include "river.hpp"
#include "biomes.hpp"

namespace VoronoiMap{
	struct TerrainGeneration{
		static void generateWater(VoronoiMap* map);
		static void generateElevation(VoronoiMap* map);
		static void generateRivers(VoronoiMap* map);
		static void generateMoisture(VoronoiMap* map);
		static void generateBiomes(VoronoiMap* map);
	};
}


#endif /* LIBADO_MAP_VORONOI_TERRAINGENERATION_HPP_ */
