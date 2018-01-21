/*Name : Jacob O'Brien
*StudentId : s3514336
*/
/*
 * aStar.cpp
 *
 *  Created on: 11 May 2016
 *      Author: linbob
 */

#include "aStar.hpp"

bool AStar::operator<(const GraphNode& l, const GraphNode& r ){
	return std::tie(l.x, l.y) < std::tie(r.x, r.y);
};

bool AStar::operator==(const GraphNode& l, const GraphNode& r ){
	return std::tie(l.x, l.y) == std::tie(r.x, r.y);
};

bool AStar::operator!=(const GraphNode& l, const GraphNode& r ){
	return std::tie(l.x, l.y) != std::tie(r.x, r.y);
};

bool AStar::operator<(const PriorityNode& l, const PriorityNode& r ){
	return l.priority < r.priority;
};

bool AStar::operator>(const PriorityNode& l, const PriorityNode& r ){
	return l.priority > r.priority;
}

std::vector<sf::Vector2i> AStar::findPath(TileMap& map, GraphNode src, GraphNode dest){
	//target tile is occupied, pick an open neighbour
	if(map.getCost(dest.x, dest.y) >= Tile::IMPASSABLE){
		//check neighbour tiles for vacancy
		if(map.getCost(dest.x, dest.y + 1) < Tile::IMPASSABLE){
			dest.y += 1;
		}else if(map.getCost(dest.x, dest.y - 1) < Tile::IMPASSABLE){
			dest.y -= 1;
		}else if(map.getCost(dest.x - 1, dest.y) < Tile::IMPASSABLE){
			dest.x -= 1;
		}else if(map.getCost(dest.x + 1, dest.y) < Tile::IMPASSABLE){
			dest.x += 1;
		}else{
			//no neighbour reachable/unoccupied, return 'no path'
			return std::vector<sf::Vector2i>();
		}
	}

	std::map<GraphNode, GraphNode> visited;	//came_from
	std::map<GraphNode, int> visitedCost;	//cost_so_far
	BinaryHeap<PriorityNode> frontier;

	frontier.insert(PriorityNode(src.x, src.y));
	visited[GraphNode(src.x, src.y)] = GraphNode(src.x, src.y);
	visitedCost[GraphNode(src.x, src.y)] = 0;

	while(!frontier.empty()){
		PriorityNode current = frontier.deleteMin();

		//GOAL HAS BEEN REACHED
		if(current == dest){
			std::vector<sf::Vector2i> path;

			GraphNode c = dest;

			while(c != src){

				path.push_back(sf::Vector2i(c.x, c.y));
				c = visited[c];
			}

			return path;
		}

		//check neigbour tiles
		for(sf::Vector2i n : map.getNeighbours(current.x, current.y)){
			//calc tile-tile travel cost
			int newCost = visitedCost[current] +
					map.getCost(n.x, n.y);

			//check if neighbour has been visited OR if tile-tile cost is cheaper
			//than previously calc'ed path cost OR if neighbour is occupied
			if((visitedCost.find(GraphNode(n.x, n.y)) == visitedCost.end() ||
					newCost < visitedCost[GraphNode(n.x, n.y)]) && newCost < Tile::IMPASSABLE){

				//add new tile-tile path cost
				visitedCost[GraphNode(n.x, n.y)] = newCost;

				int priority = newCost;

				//CALCULATE DIST FROM NEIGHBOUR TO EXIT USING MANHATTAN
				priority += std::abs(n.x - dest.x) + std::abs(n.y - dest.y);

				//SET NEW NODE COST
				frontier.insert(PriorityNode(n.x, n.y, priority));

				//mark path
				visited[GraphNode(n.x, n.y)] = GraphNode(current.x, current.y);
			}
		}
	}

	//no path found
	return std::vector<sf::Vector2i>();
}
