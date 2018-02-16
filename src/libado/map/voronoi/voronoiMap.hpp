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

		struct Vec2fComp{
		   bool operator() (const sf::Vector2f& lhs, const sf::Vector2f& rhs) const{
			   return (lhs.x > rhs.x) && (lhs.y > rhs.y);
		   }
		};

		VoronoiMap(int numCells, int mapW, int mapH);

		void generateNewMap(int numCells);
		void relaxDiagram();

		void mouseMoved(float x, float y);
		void draw(sf::RenderWindow* window);
		void toggleKingdomDraw();
		void toggleElevationDraw();
		void toggleMoistureDraw();
		void toggleNoisyEdges();

		void assignKingdomColours();

		void mouseClick(float x, float y);

		// calculates travel distance in days
		// based on average horse travel in a day of 65km/day
		int calcTravelDist(sf::Vector2f a, sf::Vector2f b){
			float dist = calcDist(a.x, a.y, b.x, b.y);
			return (int)std::round(dist / DAY_IN_KM);
		}

		// island whose radius is based on overlapping sine waves
		// Ref: https://github.com/rjanicek/voronoi-map-js/blob/master/src/island-shape.js
		// NOTE: vector Point must be in range from 0.0 - 1.0
		bool insideLandShape(sf::Vector2f point);

		Center* getClosestCell(float x, float y);
		Center* getClosestCell(sf::Vector2f vec){
			return getClosestCell(vec.x, vec.y);
		}
		CellCorner* getCorner(sf::Vector2f& point){
			return corners[{point.x, point.y}].get();
		}
		Center* getCenter(sf::Vector2f& point){
			return allCenters[{point.x, point.y}].get();
		}
		void setLandCenters(std::map<std::pair<float, float>, std::shared_ptr<Center>> list){
			landCenters = list;
		}
		std::map<std::pair<float, float>, std::shared_ptr<Center>>& getAllCenters() {
			return allCenters;
		}
		std::map<std::pair<float, float>, std::shared_ptr<Center>>& getLandCenters() {
			return landCenters;
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
		std::mt19937& getRandGen(){
			return gen;
		}
		bool isDrawingKingdoms(){
			return drawKingdoms;
		}
	private:
		std::shared_ptr<std::vector<Point2>> generateCellPoints(int numCells);

		void createGameMap();

		bool insideCircle(float x, float y, float cX, float cY, int r);

		std::shared_ptr<CellCorner> createCorner(sf::Vector2f point);

		std::shared_ptr<LineEdge<Center>> createDelaunayEdge(Edge* e);
		std::shared_ptr<LineEdge<CellCorner>> createVoronoiEdge(Edge* e);

		void addToCornerList(std::vector<std::shared_ptr<CellCorner>>& list,
				std::shared_ptr<CellCorner> c);
		void addToCenterList(std::vector<std::shared_ptr<Center>>& list,
				std::shared_ptr<Center> c);

		float calcDist(float aX, float aY, float bX, float bY){
			return std::sqrt(std::pow(aX - bX, 2) + std::pow(aY - bY, 2));
		}

		///////////////////////////////////////////

		std::map<std::pair<float, float>, std::shared_ptr<Center>> allCenters;
		std::map<std::pair<float, float>, std::shared_ptr<Center>> landCenters;
		std::map<std::pair<float, float>, std::shared_ptr<CellCorner>> corners;
		std::vector<std::shared_ptr<CellEdge>> edges;

		std::mt19937 gen;
		std::uniform_int_distribution<> xDis;
		std::uniform_int_distribution<> yDis;

		std::unique_ptr<VoronoiDiagramGenerator> generator;
		std::shared_ptr<Diagram> voronoiDiagram;

		int mapW, mapH;
		
		// used for insideLandShape
	    double ISLAND_FACTOR = 1.00;
	    int bumps ;
	    double startAngle ;
	    double dipAngle ;
	    double dipWidth ;
	    bool shapeGen = false;

		bool drawKingdoms = false;
		bool drawElevation = false;
		bool drawMoisture = false;

		bool drawNoisy = true;

		Center* firstPolyClicked = nullptr;
		Center* lastCenter = nullptr;
		Center* mouseOver = nullptr;
		sf::Color highlightColour = sf::Color::White;
	};
}

#endif /* LIBADO_MAP_VORONOI_VORONOIMAP_HPP_ */
