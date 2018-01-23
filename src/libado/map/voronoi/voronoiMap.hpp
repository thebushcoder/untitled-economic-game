/*
 * voronoiMap.hpp
 *
 *  Created on: 11 Dec 2017
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_VORONOIMAP_HPP_
#define LIBADO_MAP_VORONOI_VORONOIMAP_HPP_

#include <random>
#include <memory>
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
#include <math.h>
#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <Point2.h>
#include <Vector2.h>
#include <VoronoiDiagramGenerator.h>
#include <Edge.h>

#include "cellcorner.hpp"
#include "celledge.hpp"
#include "center.hpp"
#include "../../util/lineShape.hpp"
#include "../../util/FastNoise.h"

/*
 * 	Based on code from Amit's Polygon map gen article
 * 	Ref:
 * 	http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/#rivers
 * 	https://github.com/amitp/mapgen2/blob/master/Map.as
 */
namespace VoronoiMap{
	class VoronoiMap{
	public:
		const static int DAY_IN_PX = 21;
		const static int DAY_IN_KM = 65; // based on average horse travel in a day of 65km/day

		// calculates travel distance in days
		// based on average horse travel in a day of 65km/day
		int calcTravelDist(sf::Vector2f a, sf::Vector2f b){
			float dist = calcDist(a.x, a.y, b.x, b.y);
			return (int)std::round(dist / DAY_IN_KM);
		}

		struct Vec2fComp{
		   bool operator() (const sf::Vector2f& lhs, const sf::Vector2f& rhs) const{
			   return (lhs.x > rhs.x) && (lhs.y > rhs.y);
		   }
		};

		VoronoiMap(int numCells, int mapW, int mapH);
		void generateNewMap(int numCells);
		void relaxDiagram();
		Center* getClosestCell(float x, float y);
		Center* getClosestCell(sf::Vector2f vec){
			return getClosestCell(vec.x, vec.y);
		}
		void mouseMoved(float x, float y);
		void draw(sf::RenderWindow* window);
		void toggleKingdomDraw();
		void toggleElevationDraw();

		Center* first = nullptr;

		void mouseClick(float x, float y){
			Center* c = getClosestCell(x,y);

			if(drawElevation){
				printf("###################\n");
				printf("Clicked Poly elev: %f\n\n", c->getElevation());

				for(auto n : c->getNeighbours()){
					printf("\tneighbour Poly elev: %f\n", n->getElevation());
				}
				printf("\n");
				for(auto a : c->getCorners()){
					printf("\tPoly corner elev: %f\n", a->getElevation());
				}

				printf("###################\n");
			}else if(!first){
				first = c;
			}else{
				float d = calcDist(first->getPoint().x, first->getPoint().y,
						c->getPoint().x, c->getPoint().y);

				printf("Mouse click\n");
				printf("First: %f, %f\n", first->getPoint().x, first->getPoint().y);
				printf("Second: %f, %f\n", c->getPoint().x, c->getPoint().y);
				printf("Raw dist: %f km\n", d);
				printf("Travel dist(days): %d \n", calcTravelDist(first->getPoint(),
						c->getPoint()));
				printf("===========\n");
				first = nullptr;
			}
		}

		std::vector<CellCorner*> getLandCorners(){
			std::vector<CellCorner*> list;

			for(auto iter = corners.begin(); iter != corners.end(); ++iter){
				if(!iter->second->isWater()){
					list.push_back(iter->second.get());
				}
			}

			return list;
		}

		CellCorner* getCorner(sf::Vector2f& point){
			return corners[{point.x, point.y}].get();
		}
		Center* getCenter(sf::Vector2f& point){
			return allCenters[{point.x, point.y}].get();
		}
		std::map<std::pair<float, float>, std::shared_ptr<Center>>& getAllCenters() {
			return allCenters;
		}
		std::map<std::pair<float, float>, std::shared_ptr<CellCorner>>& getCorners() {
			return corners;
		}
		std::vector<std::shared_ptr<CellEdge> >& getEdges() {
			return edges;
		}
		Diagram* getVoronoiDiagram() {
			return voronoiDiagram.get();
		}
		void reset();

		int getMapH() const {
			return mapH;
		}
		int getMapW() const {
			return mapW;
		}
		void setDrawKingdoms(bool drawKingdoms) {
			this->drawKingdoms = drawKingdoms;
		}

	private:
		std::shared_ptr<std::vector<Point2>> generateCellPoints(int numCells);

		void createGameMap();
		void generateWater();
		void generateElevation();
		void assignPolyColours();
		bool insideCircle(float x, float y, float cX, float cY, int r);

		std::shared_ptr<CellCorner> createCorner(sf::Vector2f point);

		void addToCornerList(std::vector<std::shared_ptr<CellCorner>>& list,
				std::shared_ptr<CellCorner> c);
		void addToCenterList(std::vector<std::shared_ptr<Center>>& list,
				std::shared_ptr<Center> c);

		CenterPair createDelaunayEdge(Edge* e);
		CornerPair createVoronoiEdge(Edge* e);

	    double ISLAND_FACTOR = 1.00;
	    int bumps ;
	    double startAngle ;
	    double dipAngle ;
	    double dipWidth ;
	    bool shapeGen = false;

	    // island whose radius is based on overlapping sine waves
	    // Ref: https://github.com/rjanicek/voronoi-map-js/blob/master/src/island-shape.js
		// NOTE: vector Point must be in range from 0.0 - 1.0
		bool insideLandShape(sf::Vector2f point){
			// The min-max start angle for the sin function, in radians.
			static std::uniform_int_distribution<> doubleRange(0.0, 2 * M_PI);
			// The min-max number of sine wave "bumps" along the island.
			static std::uniform_int_distribution<> bumpRange(1, 6);
			static std::uniform_int_distribution<> dipRange(0.2, 0.7);

			if(!shapeGen){
				// The number of sine waves which form bumps along the island.
				bumps = bumpRange(gen);
				// The start angle for the sin function, in radians.
				startAngle = doubleRange(gen);
				dipAngle = doubleRange(gen);
				dipWidth = dipRange(gen);
				shapeGen = true;
			}

			// increase scale to decrease land mass size
			float radiusScale = 1;
			// normalize passed in point/vector
			point = sf::Vector2f( radiusScale * (point.x / mapW - 0.5),
					radiusScale * (point.y / mapH - 0.5));

			// Get the angle of the point from the center of the island
		    double angle = std::atan2(point.y, point.x);
		    // Get the normalized length of whichever axis is longer
		    double length = 0.5 * (std::max(std::abs(point.x), std::abs(point.y)) +
		    		std::sqrt(point.x * point.x + point.y * point.y));
		    // The inner radius has to be smaller than the length for this to be land
		    double r1 = 0.5 + 0.40 * std::sin(startAngle + bumps * angle + std::cos((bumps + 3)
		    		* angle));
		    // The outer radius has to be larger than the length for this to be land
		    double r2 = 0.3 - 0.20 * std::sin(startAngle + bumps * angle - std::sin((bumps + 2)
		    		* angle));
		    if (std::abs(angle - dipAngle) < dipWidth
		    || std::abs(angle - dipAngle + 2 * M_PI) < dipWidth
		    || std::abs(angle - dipAngle - 2 * M_PI) < dipWidth)
		    {
		    	// Our angle is less than the minimum angle
		        r1 = r2 = 0.2;
		    }

		    return (length < r1 || (length > r1 * ISLAND_FACTOR && length < r2));
		}

		float calcDist(float aX, float aY, float bX, float bY){
			float dX = bX - aX;
			float dY = bY - aY;
			return std::sqrt(dX * dX + dY * dY);
		}

		///////////////////////////////////////////

		std::map<std::pair<float, float>, std::shared_ptr<Center>> allCenters;
		std::map<std::pair<float, float>, std::shared_ptr<Center>> landCenters;
		std::map<std::pair<float, float>, std::shared_ptr<CellCorner>> corners;
		std::vector<std::shared_ptr<CellEdge>> edges;

		std::mt19937 gen;
		std::uniform_int_distribution<> xDis;
		std::uniform_int_distribution<> yDis;

		VoronoiDiagramGenerator generator;
		std::shared_ptr<Diagram> voronoiDiagram;

		int mapW, mapH;
		
		bool drawKingdoms = false;
		bool drawElevation = false;

		Center* lastCenter = nullptr;
		Center* mouseOver = nullptr;
		sf::Color highlightColour = sf::Color::White;
	};
}

#endif /* LIBADO_MAP_VORONOI_VORONOIMAP_HPP_ */
