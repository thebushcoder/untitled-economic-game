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
#include "../util/fileLogger.hpp"

class VoronoiScreen;
namespace KingdomUtil{
	struct KingdomUtil{

		static void genKingdomArea(std::map<KingdomType, anax::Entity>& kingdomList,
				VoronoiMap::VoronoiMap* map);

		static void generateKingdomVoronoi(VoronoiScreen* vorScreen);

		static void floodFill(std::queue<VoronoiMap::Center*>& front, anax::Entity e,
				VoronoiMap::VoronoiMap* map, int factionProvs);
	};
}

#endif /* LIBADO_ENTITY_KINGDOMUTIL_HPP_ */
