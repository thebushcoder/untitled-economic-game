/*
 * quadtree.hpp
 *
 *  Created on: 5 Jun 2016
 *      Author: linbob
 */

#ifndef LIBADO_UTIL_QUADTREE_HPP_
#define LIBADO_UTIL_QUADTREE_HPP_

#include <vector>
#include <memory>
#include <SFML/Graphics/Shape.hpp>

template<class T>
struct QuadObject{
	T object;
	sf::Shape* bounds;

	QuadObject(){}
	QuadObject(T& obj, sf::Shape* b) : object(obj), bounds(b){}

	bool operator==(const QuadObject<T>& rhs) const{
		return (bounds->getPosition() == rhs.bounds->getPosition());
	}
};

template<class T>
class QuadTree{
	const static int MAX_OBJECTS = 10;
	const static int MAX_LEVELS = 5;

	const static int MAX_NODES = 4;

	int level;
	sf::RectangleShape bounds;
	std::vector<QuadObject<T> > objects;
	std::vector<std::unique_ptr<QuadTree<T>> > nodes;
public:
	QuadTree(){}
	QuadTree(int s_level, int x, int y, int w, int h) :
			level(s_level), bounds(sf::Vector2f(w,y)), objects(), nodes(MAX_NODES){
		bounds.setPosition(x, y);
	}

	void clear(){
		objects.clear();

		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i] != nullptr) {
				nodes[i]->clear();
				nodes[i] = nullptr;
			}
		}
	}
	void split() {
		int subWidth = (int)(bounds.getGlobalBounds().width / 2);
		int subHeight = (int)(bounds.getGlobalBounds().height / 2);
		int x = (int)bounds.getGlobalBounds().left;
		int y = (int)bounds.getGlobalBounds().top;

		nodes[0] = std::unique_ptr<QuadTree<T>>(new QuadTree(level + 1,
				x + subWidth, y, subWidth, subHeight));
		nodes[1] = std::unique_ptr<QuadTree<T>>(new QuadTree(level + 1,
				x + subWidth, y, subWidth, subHeight));
		nodes[2] = std::unique_ptr<QuadTree<T>>(new QuadTree(level + 1,
				x + subWidth, y, subWidth, subHeight));
		nodes[3] = std::unique_ptr<QuadTree<T>>(new QuadTree(level + 1,
				x + subWidth, y, subWidth, subHeight));
	}
	/* Determine which node the object belongs to. -1 means
	 * object cannot completely fit within a child node and is part
	 * of the parent node
	 */
	int getIndex(sf::Shape* objectBounds){
		int index = -1;
		double verticalMidpoint = bounds.getGlobalBounds().left + (bounds.getGlobalBounds().width / 2);
		double horizontalMidpoint = bounds.getGlobalBounds().top + (bounds.getGlobalBounds().height / 2);

		// Object can completely fit within the top quadrants
		bool topQuadrant = (objectBounds->getGlobalBounds().top < horizontalMidpoint &&
				objectBounds->getGlobalBounds().top + objectBounds->getGlobalBounds().height <
				horizontalMidpoint);
		// Object can completely fit within the bottom quadrants
		bool bottomQuadrant = (objectBounds->getGlobalBounds().top > horizontalMidpoint);

		// Object can completely fit within the left quadrants
		if (objectBounds->getGlobalBounds().left < verticalMidpoint &&
				objectBounds->getGlobalBounds().left + objectBounds->getGlobalBounds().width <
				verticalMidpoint) {
			if (topQuadrant) {
				index = 1;
			}
			else if (bottomQuadrant) {
				index = 2;
			}
		}
		// Object can completely fit within the right quadrants
		else if (objectBounds->getGlobalBounds().left > verticalMidpoint) {
			if (topQuadrant) {
				index = 0;
			}
			else if (bottomQuadrant) {
				index = 3;
			}
		}

		return index;
	}
	//object must have a testable bounds box
	void insert(QuadObject<T> object){
		if (nodes[0] != nullptr) {
			int index = getIndex(object.bounds);

			if (index != -1) {
				nodes[index]->insert(object);

				return;
			}
		}

		objects.push_back(object);

		if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
			if (nodes[0] == nullptr){
				split();
			}

			int i = 0;
			while (i < objects.size()) {
				int index = getIndex(objects[i].bounds);
				if (index != -1) {
					nodes[index]->insert(objects[i]);
					objects.erase(objects.begin() + i);
				}else{
					i++;
				}
			}
		}
	}
	//returns a list of all objects that could collide with objectBounds
	std::vector<T> retrieve(std::vector<T>& returnObjects, sf::Shape* objectBounds){
		int index = getIndex(objectBounds);

		if (index != -1 && nodes[0] != nullptr) {
			nodes[index]->retrieve(returnObjects, objectBounds);
		}

		for(QuadObject<T>& o : objects){
			returnObjects.push_back(o.object);
		}

		return returnObjects;
	}
};



#endif /* LIBADO_UTIL_QUADTREE_HPP_ */
