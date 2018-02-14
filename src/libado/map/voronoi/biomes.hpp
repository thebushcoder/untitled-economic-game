/*
 * biomes.hpp
 *
 *  Created on: 7 Feb 2018
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_BIOMES_HPP_
#define LIBADO_MAP_VORONOI_BIOMES_HPP_

namespace Biomes{
	enum Biome{
		EMPTY,
		BEACH,
		DESERT,
		GRASSLAND,
		FOREST,
		RAIN_FOREST,
		ROCKY_MOUNT,
		SNOWY_MOUNT,
		LAKE
	};

	static sf::Color getColor(Biome b){
		switch(b){
		case BEACH:
			return sf::Color(170, 191, 0);
			break;
		case DESERT:
			return sf::Color(164, 176, 56);
			break;
		case GRASSLAND:
			return sf::Color(48, 152, 70);
			break;
		case FOREST:
			return sf::Color(29, 93, 42);
			break;
		case RAIN_FOREST:
			return sf::Color(15, 71, 27);
			break;
		case ROCKY_MOUNT:
			return sf::Color(77, 77, 77);
			break;
		case SNOWY_MOUNT:
			return sf::Color(200, 200, 200);
			break;
		case LAKE:
			return sf::Color::Blue;
			break;
		}
		return sf::Color::Black;
	}
}

#endif /* LIBADO_MAP_VORONOI_BIOMES_HPP_ */
