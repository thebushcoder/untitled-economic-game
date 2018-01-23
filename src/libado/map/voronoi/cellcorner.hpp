/*
 * Corner.hpp
 *
 *  Created on: 11 Dec 2017
 *      Author: linbob
 */

#ifndef LIBADO_MAP_VORONOI_CELLCORNER_HPP_
#define LIBADO_MAP_VORONOI_CELLCORNER_HPP_

#include "center.hpp"
#include "celledge.hpp"

namespace VoronoiMap{
	class CellCorner{
	public:
		CellCorner(sf::Vector2f point) : point(point){}
		~CellCorner(){}

		void setMapBorder(bool flag){ mapBorder = flag; }

		bool isMapBorder() {
			return mapBorder;
		}
		sf::Vector2f getPoint() {
			return point;
		}
		std::vector<std::shared_ptr<CellEdge>>& getProtrudes() {
			return protrudes;
		}
		std::vector<std::shared_ptr<Center>>& getTouches() {
			return touches;
		}
		std::vector<std::shared_ptr<CellCorner>>& getAdjacent() {
			return adjacent;
		}
		float getElevation() {
			return elevation;
		}
		void setElevation(float elevation) {
			this->elevation = elevation;
		}
		void addTouchingCenter(std::shared_ptr<Center> c){
			touches.push_back(c);
		}
		void addProtrudingEdge(std::shared_ptr<CellEdge> e){
			protrudes.push_back(e);
		}
		void addNeighbourCorner(std::shared_ptr<CellCorner> c){
			adjacent.push_back(c);
		}
		bool isRiver(){
			return river > 0;
		}
		bool isWater(){
			return water;
		}
		void setIsWater(bool isWater){
			this->water = isWater;
		}
		bool operator ==(const CellCorner &b) const{
			return (this->point.x == b.point.x) && (this->point.y == b.point.y);
		}
		bool operator <(const CellCorner& right){
			return (this->point.x < right.point.x) && (this->point.y < right.point.y);
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
		}
		std::shared_ptr<CellCorner>& getDownSlope() {
			return downSlope;
		}
		void setDownSlope(std::shared_ptr<CellCorner> const& downSlope) {
			this->downSlope = downSlope;
		}
		float getMoisture() const {
			return moisture;
		}
		void setMoisture(float moisture = 0.0) {
			this->moisture = moisture;
		}
		void setRiver(int river = 0) {
			this->river = river;
		}
		int getRiver(){
			return river;
		}
		std::shared_ptr<CellCorner>& getWaterShed(){
			return waterShed;
		}
		void setWaterShed(std::shared_ptr<CellCorner> const& waterShed) {
				this->waterShed = waterShed;
		}
		int getWaterShedSize() const {
			return waterShedSize;
		}
		void setWaterShedSize(int waterShedSize = 0) {
			this->waterShedSize = waterShedSize;
		}
		CellEdge* getDownslopeEdge();

	private:
		bool mapBorder = false;
		bool water = false;
		bool ocean = false;
		bool coast = false;
		float elevation = 0.0;
		float moisture = 0.0;
		int river = 0;
		int waterShedSize = 0;

		std::shared_ptr<CellCorner> downSlope = nullptr;
		std::shared_ptr<CellCorner> waterShed = nullptr;

		sf::Vector2f point;

		std::vector<std::shared_ptr<Center>> touches; // nearby center points
		std::vector<std::shared_ptr<CellEdge>> protrudes; // edges protruding from corner point, like spokes on a wheel
		std::vector<std::shared_ptr<CellCorner>> adjacent;	// neighbouring corner points
	};
}

#endif /* LIBADO_MAP_VORONOI_CELLCORNER_HPP_ */
