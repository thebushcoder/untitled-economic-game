/*Name : Jacob O'Brien
*StudentId : s3514336
*/
/*
 * aStar.hpp
 *
 *  Created on: 11 May 2016
 *      Author: linbob
 */

#ifndef SRC_SOLVE_ASTAR_HPP_
#define SRC_SOLVE_ASTAR_HPP_

#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../map/tileMap.hpp"
#include "binaryHeap.hpp"

/*	TILE COSTS: 0(ice/grease, very fast)-99(wall/im-passable)
 *
 */

namespace AStar{
	struct GraphNode{
		int x, y;

		GraphNode(int x = 0, int y = 0) : x(x), y(y){};
	};

	struct PriorityNode : GraphNode{
		float priority;

		PriorityNode(int x = 0, int y = 0, float priority = 0) :
			GraphNode(x, y), priority(priority){}
	};

	bool operator<(const GraphNode& l, const GraphNode& r );

	bool operator==(const GraphNode& l, const GraphNode& r );

	bool operator!=(const GraphNode& l, const GraphNode& r );

	bool operator<(const PriorityNode& l, const PriorityNode& r );

	bool operator>(const PriorityNode& l, const PriorityNode& r );

	std::vector<sf::Vector2i> findPath(TileMap& map, GraphNode src, GraphNode dest);

};

#endif /* SRC_SOLVE_ASTAR_HPP_ */
