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
#include "river.hpp"

namespace VoronoiMap{
	class CellCorner{
	public:
		CellCorner() {}
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
		bool isWater(){
			return water;
		}
		void setIsWater(bool isWater){
			this->water = isWater;
		}
		bool operator ==(const CellCorner &b){
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
		float getMoisture() const {
			return moisture;
		}
		void setMoisture(float moisture = 0.0) {
			this->moisture = moisture;
		}
		CellEdge* getEdge(CellCorner* n);

		std::shared_ptr<CellEdge>& getRiverEdge(){
			return riverEdge;
		}

		void setRiverEdge(std::shared_ptr<CellEdge> river) {
			this->riverEdge = river;
		}
		std::shared_ptr<CellCorner>& getDownStream(){
			return downStream;
		}
		void setDownStream(std::shared_ptr<CellCorner> downStream) {
			this->downStream = downStream;
		}
		std::shared_ptr<CellCorner> getUpstream(){
			return upstream;
		}
		void setUpstream(std::shared_ptr<CellCorner> upstream) {
			this->upstream = upstream;
		}
		std::shared_ptr<River>& getRiver();
		void setRiver(std::shared_ptr<River> river);

	private:
		bool mapBorder = false;
		bool water = false;
		bool ocean = false;
		bool coast = false;
		float elevation = 0.0;
		float moisture = 0.0;

		sf::Vector2f point;

		std::shared_ptr<CellCorner> upstream;
		std::shared_ptr<CellCorner> downStream;
		std::shared_ptr<CellEdge> riverEdge;
		std::shared_ptr<River> river;

		std::vector<std::shared_ptr<Center>> touches; // nearby center points
		std::vector<std::shared_ptr<CellEdge>> protrudes; // edges protruding from corner point, like spokes on a wheel
		std::vector<std::shared_ptr<CellCorner>> adjacent;	// neighbouring corner points
	};
}

#endif /* LIBADO_MAP_VORONOI_CELLCORNER_HPP_ */
