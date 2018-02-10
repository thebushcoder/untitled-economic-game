#ifndef LIBADO_MAP_VORONOI_CENTER_HPP_
#define LIBADO_MAP_VORONOI_CENTER_HPP_

#include <memory>
#include <fstream>
#include <iomanip>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <Point2.h>
#include <Vector2.h>
#include <VoronoiDiagramGenerator.h>

#include "../../entity/kingdomType.hpp"
#include "biomes.hpp"

namespace VoronoiMap{
	class CellEdge;
	class CellCorner;
	class Center{
	public:
		Center(sf::Vector2f point) : point(point), polyShape(new sf::ConvexShape()){
//			polyShape->setOutlineThickness(2);
		}
		~Center(){}

		CellEdge* getNextVorEdge(std::vector<CellEdge*>* visited, sf::Vector2f nextPoint);

		std::vector<std::shared_ptr<CellCorner>>& getCorners() {
			return corners;
		}
		std::vector<std::shared_ptr<Center>>& getNeighbours() {
			return neighbours;
		}
		sf::Vector2f getPoint() {
			return point;
		}
		std::vector<std::shared_ptr<CellEdge>>& getPolyEdges() {
			return polyEdges;
		}
		sf::ConvexShape& getPolyShape() {
			return *polyShape;
		}
		void addNeighbour(std::shared_ptr<Center> n){
			neighbours.push_back(n);
		}
		void addEdge(std::shared_ptr<CellEdge> e){
			polyEdges.push_back(e);
		}
		void addCorner(std::shared_ptr<CellCorner> c){
			corners.push_back(c);
		}
		bool isWater() {
			return water;
		}
		void setIsWater(bool isWater) {
			this->water = isWater;
			if(isWater){
				setColour(sf::Color::Blue);
			}
		}
		void setColour(sf::Color c){
			shapeCol = c;
			polyShape->setFillColor(c);
		}
		void reApplyColour(){
			polyShape->setFillColor(shapeCol);
		}
		KingdomUtil::KingdomType getOwner(){
			return owner;
		}
		float getElevation(){
			return elevation;
		}
		void setElevation(float elevation){
			this->elevation = elevation;
		}
		void setOwner(KingdomUtil::KingdomType owner) {
			this->owner = owner;
		}
		friend bool operator <(const Center& left, const Center& right){
			return (left.point.x < right.point.x) && (left.point.y < right.point.y);
		}
		bool operator ==(const Center &b) const{
			return (this->point.x == b.point.x) && (this->point.y == b.point.y);
		}
		static bool checkEdgePoint(sf::Vector2f corner, sf::Vector2f next);
		bool isBorder() {
			return border;
		}
		void setIsBorder(bool isBorder){
			this->border = isBorder;
		}
		bool isCoast() const {
			return coast;
		}

		void setIsCoast(bool isCoast = false) {
			this->coast = isCoast;
		}

		bool isOcean() const {
			return ocean;
		}

		void setIsOcean(bool isOcean = false) {
			this->ocean = isOcean;
			setIsWater(isOcean);
		}
		float getMoisture() const {
			return moisture;
		}
		void setMoisture(float moisture = 0.0) {
			this->moisture = moisture;
		}
		CellEdge* getDelEdge(Center* neighbour);
		Biomes::Biome getBiome() const {
			return biome;
		}
		void setBiome(Biomes::Biome biome) {
			this->biome = biome;
		}

		float nMoist = 0.0;
		float eMoist = 0.0;
		float wMoist = 0.0;


	private:
		sf::Vector2f point;

		std::vector<std::shared_ptr<Center>> neighbours;
		std::vector<std::shared_ptr<CellEdge>> polyEdges;
		std::vector<std::shared_ptr<CellCorner>> corners;

		std::unique_ptr<sf::ConvexShape> polyShape;
		sf::Color shapeCol;

		Biomes::Biome biome;
		bool water = false;
		bool ocean = false;
		bool coast = false;
		bool border = false;
		float elevation = 0.0;
		float moisture = 0.0;
		KingdomUtil::KingdomType owner = KingdomUtil::KingdomType::NONE;	// kingdom
	};
}

#endif /* LIBADO_MAP_VORONOI_CENTER_HPP_ */
