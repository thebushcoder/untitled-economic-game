/*
 * voronoiMap.cpp
 *
 *  Created on: 11 Dec 2017
 *      Author: linbob
 */

#include "voronoiMap.hpp"

namespace VoronoiMap{

	VoronoiMap::VoronoiMap(int numCells, int mapW, int mapH) : mapW(mapW), mapH(mapH){
		std::random_device rd;
		gen = std::mt19937(rd());
		xDis = std::uniform_int_distribution<>(0, mapW);
		yDis = std::uniform_int_distribution<>(0, mapH);

		generator = std::unique_ptr<VoronoiDiagramGenerator>(new VoronoiDiagramGenerator());

		generateNewMap(numCells);
	}
	void VoronoiMap::generateNewMap(int numCells){
		std::shared_ptr<std::vector<Point2>> points = generateCellPoints(numCells);
		voronoiDiagram = std::shared_ptr<Diagram>(
				generator->compute(*points, BoundingBox(0, mapW, mapH, 0))
				);

		int numRelax = 3;
		for(int i = 0; i < numRelax; ++i){
			relaxDiagram();
		}
	}
	void VoronoiMap::relaxDiagram(){
		Diagram* relaxedDiagram = generator->relax();

		reset();

		voronoiDiagram.reset(relaxedDiagram);

		createGameMap();
	}
	std::shared_ptr<std::vector<Point2>> VoronoiMap::generateCellPoints(int numCells){
		std::shared_ptr<std::vector<Point2>> points = std::make_shared<std::vector<Point2>>();

		Point2 s;

		for(int i = 0; i < numCells; ++i){
			s.x = xDis(gen);
			s.y = yDis(gen);
			points->push_back(s);
		}

		return points;
	}
	void VoronoiMap::createGameMap(){
		shapeGen = false;

		for(auto cell : voronoiDiagram->cells){
			sf::Vector2f point(cell->site.p.x, cell->site.p.y);
			std::shared_ptr<Center> gameCell = std::make_shared<Center>(point);
			allCenters[{point.x, point.y}] = gameCell;
		}
		for(auto e : voronoiDiagram->edges){
			std::shared_ptr<CellEdge> gameEdge = std::make_shared<CellEdge>();
			edges.push_back(gameEdge);

			gameEdge->setDelaunayEdge(createDelaunayEdge(e));
			gameEdge->setVoronoiEdge(createVoronoiEdge(e));

			// add edges to centers
			if(gameEdge->getDelaunayEdge()->getPointA()){
				gameEdge->getDelaunayEdge()->getPointA()->addEdge(gameEdge);
			}
			if(gameEdge->getDelaunayEdge()->getPointB()){
				gameEdge->getDelaunayEdge()->getPointB()->addEdge(gameEdge);
			}

			// add protruding edge to corners
			if(gameEdge->getVoronoiEdge()->getPointA()){
				gameEdge->getVoronoiEdge()->getPointA()->addProtrudingEdge(gameEdge);
			}
			if(gameEdge->getVoronoiEdge()->getPointB()){
				gameEdge->getVoronoiEdge()->getPointB()->addProtrudingEdge(gameEdge);
			}

			// add one delaunay center to the neighbour list of the other delaunay center
			if(gameEdge->getDelaunayEdge()->getPointA() && gameEdge->getDelaunayEdge()->getPointB()){
				addToCenterList(gameEdge->getDelaunayEdge()->getPointA()->getNeighbours(),
						gameEdge->getDelaunayEdge()->getPointB());
				addToCenterList(gameEdge->getDelaunayEdge()->getPointB()->getNeighbours(),
						gameEdge->getDelaunayEdge()->getPointA());
			}
			// add voronoi edge corners to the adjacent list of the opposite corner
			if(gameEdge->getVoronoiEdge()->getPointA() && gameEdge->getVoronoiEdge()->getPointB()){
				addToCornerList(gameEdge->getVoronoiEdge()->getPointA()->getAdjacent(),
						gameEdge->getVoronoiEdge()->getPointB());
				addToCornerList(gameEdge->getVoronoiEdge()->getPointB()->getAdjacent(),
						gameEdge->getVoronoiEdge()->getPointA());
			}

			// if a delaunay point exists(in the case of map borders),
			// add voronoi corners to the delaunay center's corner list
			if(gameEdge->getDelaunayEdge()->getPointA()){
				addToCornerList(gameEdge->getDelaunayEdge()->getPointA()->getCorners(),
						gameEdge->getVoronoiEdge()->getPointA());
				addToCornerList(gameEdge->getDelaunayEdge()->getPointA()->getCorners(),
						gameEdge->getVoronoiEdge()->getPointB());
			}

			if(gameEdge->getDelaunayEdge()->getPointB()){
				addToCornerList(gameEdge->getDelaunayEdge()->getPointB()->getCorners(),
						gameEdge->getVoronoiEdge()->getPointA());
				addToCornerList(gameEdge->getDelaunayEdge()->getPointB()->getCorners(),
						gameEdge->getVoronoiEdge()->getPointB());
			}

			// if a voronoi point exists(in the case of map borders),
			// add delaunay center to the voronoi corner's edge list
			if(gameEdge->getVoronoiEdge()->getPointA()){
				addToCenterList(gameEdge->getVoronoiEdge()->getPointA()->getTouches(),
						gameEdge->getDelaunayEdge()->getPointA());
				addToCenterList(gameEdge->getVoronoiEdge()->getPointA()->getTouches(),
						gameEdge->getDelaunayEdge()->getPointB());
			}
			if(gameEdge->getVoronoiEdge()->getPointB()){
				addToCenterList(gameEdge->getVoronoiEdge()->getPointB()->getTouches(),
						gameEdge->getDelaunayEdge()->getPointA());
				addToCenterList(gameEdge->getVoronoiEdge()->getPointB()->getTouches(),
						gameEdge->getDelaunayEdge()->getPointB());
			}
		}

		// CREATE POLYGONS FOR CELLS(CENTERs)
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			int s = iter->second->getPolyEdges().size();
			if(s <= 0) continue;

			iter->second->getLinePoly().setPointCount(s);

			CellEdge* e = iter->second->getPolyEdges()[0].get();
			sf::Vector2f current = e->getVoronoiEdge()->getPointA()->getPoint();
			sf::Vector2f nextPoint = e->getVoronoiEdge()->getPointB()->getPoint();
			std::vector<CellEdge*> visited;

			// walk corner points and construct polygon
			for(int i = 0; i < iter->second->getPolyEdges().size(); ++i){
				if(!e){
					break;
				}
				if(i > 0){
					if(Center::checkEdgePoint(e->getVoronoiEdge()->getPointA()->getPoint(), nextPoint)){
						current = e->getVoronoiEdge()->getPointA()->getPoint();
						nextPoint = e->getVoronoiEdge()->getPointB()->getPoint();
					}else{
						current = e->getVoronoiEdge()->getPointB()->getPoint();
						nextPoint = e->getVoronoiEdge()->getPointA()->getPoint();
					}
				}

				iter->second->getLinePoly().setPoint(i, current);

				visited.push_back(e);
				e = iter->second->getNextVorEdge(&visited, nextPoint);
			}
//			iter->second->setColour(sf::Color(51, 102, 0));
		}
	}

	void VoronoiMap::draw(sf::RenderWindow* window){
		// draw polygons
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){

			if(drawNoisy){
				window->draw(iter->second->getNoisyPoly());
			}else{
				window->draw(iter->second->getLinePoly());
			}

			sf::RectangleShape point(sf::Vector2f(2, 2));
			point.setPosition(iter->second->getPoint());
			point.setFillColor(sf::Color::White);
			window->draw(point);
		}
		// draw rivers
		for(auto e : edges){
			if(!e->isRiver()) continue;

			if(e->getNoisyLine().size() > 0){
				for(int i = 0; i < e->getNoisyLine().size(); ++i){
					sf::LineShape* line = e->getNoisyLine()[i].get();
					window->draw(*line);
				}
			}else{
				window->draw(*e->getVorLine());
			}
		}
	}

	void VoronoiMap::assignKingdomColours(){
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			switch(iter->second->getOwner()){
			case KingdomUtil::KingdomType::HUMAN:
			{
				sf::Color orange(255, 179, 102, 185);
				iter->second->setColour(orange);
				break;
			}
			case KingdomUtil::KingdomType::DWARF:
			{
				sf::Color blue(102, 163, 255, 185);
				iter->second->setColour(blue);
				break;
			}
			case KingdomUtil::KingdomType::ELF:
			{
				sf::Color green(77, 255, 77, 185);
				iter->second->setColour(green);
				break;
			}
			case KingdomUtil::KingdomType::REPUB:
			{
				sf::Color red(255, 102, 102, 185);
				iter->second->setColour(red);
				break;
			}
			}
		}
	}

	void VoronoiMap::toggleElevationDraw(){
		if(!drawElevation){
			for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
				if(iter->second->getElevation() < 0.0){
					sf::Color colour(255, 0, 0);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getElevation() == 0.0){
					sf::Color colour(0, 0, 0);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getElevation() <= 0.2){
					sf::Color colour(50, 50, 50);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getElevation() <= 0.4){
					sf::Color colour(100, 100, 100);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getElevation() <= 0.6){
					sf::Color colour(165, 165, 165);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getElevation() <= 0.8){
					sf::Color colour(200, 200, 200);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getElevation() <= 1.0){
					sf::Color colour(255, 255, 255);
					iter->second->setPolyColour(colour);
				}
			}
			drawElevation = true;
		}else{
			for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
				iter->second->reApplyColour();
			}
			drawElevation = false;
		}
	}
	void VoronoiMap::toggleMoistureDraw(){
		if(!drawMoisture){
			for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
				if(iter->second->getMoisture() <= 0.0){
					sf::Color colour(0, 0, 0);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getMoisture() <= 0.2){
					sf::Color colour(0, 0, 50);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getMoisture() <= 0.4){
					sf::Color colour(0, 0, 100);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getMoisture() <= 0.6){
					sf::Color colour(0, 0, 165);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getMoisture() <= 0.8){
					sf::Color colour(0, 0, 200);
					iter->second->setPolyColour(colour);
				}else if(iter->second->getMoisture() <= 1.0 ||
						iter->second->getMoisture() > 1.0){
					sf::Color colour(0, 0, 255);
					iter->second->setPolyColour(colour);
				}
			}
			drawMoisture = true;
		}else{
			for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
				iter->second->reApplyColour();
			}
			drawMoisture = false;
		}
	}
	void VoronoiMap::toggleNoisyEdges(){
		drawNoisy = !drawNoisy;
	}

	void VoronoiMap::toggleKingdomDraw(){
		drawKingdoms = !drawKingdoms;
	}
	void VoronoiMap::mouseMoved(float x, float y){
		if(x >= 0 && y >= 0 && x <= mapW && y <= mapH){
			Center* c = getClosestCell(x, y);
			if(c && c != mouseOver){
				if(mouseOver) mouseOver->reApplyColour();

				c->setPolyColour(highlightColour);
				mouseOver = c;
			}
		}else if(mouseOver){
			mouseOver->reApplyColour();
			mouseOver = nullptr;
		}
	}
	Center* VoronoiMap::getClosestCell(float x, float y){
		int radius = 50;
		Center* next;
		Center* current;

		// select starting cell; random if no previous cell selected
		std::uniform_int_distribution<> iDis(0, allCenters.size() - 1);
		if(lastCenter){
			next = lastCenter;
		}else{
			auto iter = allCenters.begin();
			std::advance(iter, iDis(gen));

			next = iter->second.get();
		}

		current = next;

		// calc cell(center) distance from mouse position
		float dist = calcDist(x, y, current->getPoint().x, current->getPoint().y);

		// loop neighbouring cells, calc dist and set neighbour to current if
		// it is closer to mouse position
		do{
			current = next;
			next = nullptr;

			for(auto e : current->getPolyEdges()){
				Center* dEdgePoint = e->getDelaunayEdge()->getPointA().get();
				if(dEdgePoint->getPoint() == current->getPoint() || !dEdgePoint){
					dEdgePoint = e->getDelaunayEdge()->getPointB().get();
				}
				if(dEdgePoint){
					float nDist = calcDist(x, y, dEdgePoint->getPoint().x, dEdgePoint->getPoint().y);
					if(nDist < dist){
						dist = nDist;
						next = dEdgePoint;
						break;
					}
				}
			}
		}while(next);

		if(current != lastCenter){
			lastCenter = current;
		}

		if(dist < radius * radius) return current;

		return nullptr;
	}
	std::shared_ptr<LineEdge<CellCorner>> VoronoiMap::createVoronoiEdge(Edge* e){
		std::shared_ptr<LineEdge<CellCorner>> vEdge = std::make_shared<LineEdge<CellCorner>>();

		if(e->vertA){
			sf::Vector2f v1Point(e->vertA->x, e->vertA->y);
			vEdge->setPointA(createCorner(v1Point));
		}
		if(e->vertB){
			sf::Vector2f v2Point(e->vertB->x, e->vertB->y);
			vEdge->setPointB(createCorner(v2Point));
		}
		return vEdge;
	}
	void VoronoiMap::reset(){
		corners.clear();
		edges.clear();
		allCenters.clear();
		landCenters.clear();
		drawElevation = false;
		drawKingdoms = false;
		lastCenter = nullptr;
		mouseOver = nullptr;
		std::random_device rd;
		gen = std::mt19937(rd());
	}
	std::shared_ptr<LineEdge<Center>> VoronoiMap::createDelaunayEdge(Edge* e){
		std::shared_ptr<LineEdge<Center>> dEdge = std::make_shared<LineEdge<Center>>();
		if(e->lSite){
			dEdge->setPointA(allCenters[{e->lSite->p.x, e->lSite->p.y}]);
		}
		if(e->rSite){
			dEdge->setPointB(allCenters[{e->rSite->p.x, e->rSite->p.y}]);
		}
		return dEdge;
	}
	// adds corner to list if !=null and corner isnt in list
	void VoronoiMap::addToCornerList(std::vector<std::shared_ptr<CellCorner>>& list,
			std::shared_ptr<CellCorner> c){

		if(c && (std::find(list.begin(), list.end(), c) == list.end())){
			list.push_back(c);
		}
	}
	// adds center to list if !=null and center isnt in list
	void VoronoiMap::addToCenterList(std::vector<std::shared_ptr<Center>>& list,
			std::shared_ptr<Center> c){

		if(c && (std::find(list.begin(), list.end(), c) == list.end())){
			list.push_back(c);
		}
	}
	bool VoronoiMap::insideCircle(float x, float y, float cX, float cY, int r){
		int dx = std::abs(x-cX);
		if(dx > r) return false;
		int dy = std::abs(y-cY);
		if(dy > r) return false;
		if(dx + dy <= r) return true;
		return (dx * dx + dy * dy <= r * r);
	}
	std::shared_ptr<CellCorner> VoronoiMap::createCorner(sf::Vector2f point){
		if(corners[{point.x, point.y}]) return corners[{point.x, point.y}];

		std::shared_ptr<CellCorner> corner = std::make_shared<CellCorner>(point);
		if(point.x <= 0 || point.y <= 0 || point.x >= mapW || point.y >= mapH){
			corner->setMapBorder(true);
		}
		corners[{point.x, point.y}] = corner;

		return corner;
	}
	bool VoronoiMap::insideLandShape(sf::Vector2f point){
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
	void VoronoiMap::mouseClick(float x, float y){
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
		}else if(drawMoisture){
			printf("###################\n");
			printf("Clicked Poly moist: %f\n", c->getMoisture());
			printf("\tNoise moist: %f\n", c->nMoist);
			printf("\tElevation moist: %f\n", c->eMoist);
			printf("\twater src moist: %f\n", c->wMoist);

			printf("###################\n");
		}else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
			printf("Clicked biome: %d\n", c->getBiome());
			for(auto e : c->getPolyEdges()){
				auto a = e->getVoronoiEdge()->getPointA()->getPoint();
				auto b = e->getVoronoiEdge()->getPointB()->getPoint();
				printf("\tVoronoi Edge length: %f\n", calcDist(a.x, a.y, b.x, b.y));
			}

		}else if(!firstPolyClicked){
			firstPolyClicked = c;
		}else{
			float d = calcDist(firstPolyClicked->getPoint().x, firstPolyClicked->getPoint().y,
					c->getPoint().x, c->getPoint().y);

			printf("Mouse click\n");
			printf("First: %f, %f\n", firstPolyClicked->getPoint().x,
					firstPolyClicked->getPoint().y);
			printf("Second: %f, %f\n", c->getPoint().x, c->getPoint().y);
			printf("Raw dist: %f km\n", d);
			printf("Travel dist(days): %d \n",
					calcTravelDist(firstPolyClicked->getPoint(), c->getPoint()));
			printf("===========\n");
			firstPolyClicked = nullptr;
		}
	}
}
