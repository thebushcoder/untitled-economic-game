/*
 * placement.cpp
 *
 *  Created on: 25 Mar 2016
 *      Author: linbob
 */

#include "placement.hpp"

PlacementManager::PlacementManager(WorldView* v, sf::RenderWindow* w) :
	current(nullptr){}
bool PlacementManager::leftClick(WorldView* v, sf::RenderWindow* w){
	if(current != nullptr && current->checkConditions()){
		sf::Vector2f converted = w->mapPixelToCoords(sf::Mouse::getPosition(*w), *v->view);

		int x = std::round(converted.x / TILESIZE);
		int y = std::round(converted.y / TILESIZE);

		return current->place(x, y);
	}
	return false;
}
bool PlacementManager::rightClick(){
	if(current != nullptr && current->checkConditions()){
		current->reset();
		return true;
	}
	return false;
}
bool PlacementManager::mouseMoved(WorldView* v, sf::RenderWindow* w){
	if(current != nullptr && current->checkConditions()){
		sf::Vector2f converted = w->mapPixelToCoords(sf::Mouse::getPosition(*w), *v->view);

		int x = std::round(converted.x / TILESIZE);
		int y = std::round(converted.y / TILESIZE);

		if(lastPlaced.x != x || lastPlaced.y != y){
			if(current->place(x, y)){
				lastPlaced.x = x;
				lastPlaced.y = y;
				return true;
			}
		}
	}

	return false;
}
void PlacementManager::addPlaceable(std::string name, std::unique_ptr<Placeable> p){
	list.insert(std::make_pair(name, std::move(p)));
}
Placeable* PlacementManager::getCurrent(){
	return current;
}
void PlacementManager::setCurrent(std::string name){
	current = list[name].get();
}
void PlacementManager::reset(){
	current = nullptr;
}
void PlacementManager::render(sf::RenderWindow& window){
	if(current == nullptr) return;

	current->render(window);
}
Placeable* PlacementManager::getPlaceable(std::string name){
	return list[name].get();
}
