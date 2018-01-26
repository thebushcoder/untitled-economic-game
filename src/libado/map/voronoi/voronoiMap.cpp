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

		voronoiDiagram = std::shared_ptr<Diagram>(relaxedDiagram);

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
			if(gameEdge->getDelaunayEdge().first){
				gameEdge->getDelaunayEdge().first->addEdge(gameEdge);
			}
			if(gameEdge->getDelaunayEdge().second){
				gameEdge->getDelaunayEdge().second->addEdge(gameEdge);
			}

			// add protruding edge to corners
			if(gameEdge->getVoronoiEdge().first){
				gameEdge->getVoronoiEdge().first->addProtrudingEdge(gameEdge);
			}
			if(gameEdge->getVoronoiEdge().second){
				gameEdge->getVoronoiEdge().second->addProtrudingEdge(gameEdge);
			}

			// add one delaunay center to the neighbour list of the other delaunay center
			if(gameEdge->getDelaunayEdge().first && gameEdge->getDelaunayEdge().second){
				addToCenterList(gameEdge->getDelaunayEdge().first->getNeighbours(),
						gameEdge->getDelaunayEdge().second);
				addToCenterList(gameEdge->getDelaunayEdge().second->getNeighbours(),
						gameEdge->getDelaunayEdge().first);
			}
			// add voronoi edge corners to the adjacent list of the opposite corner
			if(gameEdge->getVoronoiEdge().first && gameEdge->getVoronoiEdge().second){
				addToCornerList(gameEdge->getVoronoiEdge().first->getAdjacent(),
						gameEdge->getVoronoiEdge().second);
				addToCornerList(gameEdge->getVoronoiEdge().second->getAdjacent(),
						gameEdge->getVoronoiEdge().first);
			}

			// if a delaunay point exists(in the case of map borders),
			// add voronoi corners to the delaunay center's corner list
			if(gameEdge->getDelaunayEdge().first){
				addToCornerList(gameEdge->getDelaunayEdge().first->getCorners(),
						gameEdge->getVoronoiEdge().first);
				addToCornerList(gameEdge->getDelaunayEdge().first->getCorners(),
						gameEdge->getVoronoiEdge().second);
			}

			if(gameEdge->getDelaunayEdge().second){
				addToCornerList(gameEdge->getDelaunayEdge().second->getCorners(),
						gameEdge->getVoronoiEdge().first);
				addToCornerList(gameEdge->getDelaunayEdge().second->getCorners(),
						gameEdge->getVoronoiEdge().second);
			}

			// if a voronoi point exists(in the case of map borders),
			// add delaunay center to the voronoi corner's edge list
			if(gameEdge->getVoronoiEdge().first){
				addToCenterList(gameEdge->getVoronoiEdge().first->getTouches(),
						gameEdge->getDelaunayEdge().first);
				addToCenterList(gameEdge->getVoronoiEdge().first->getTouches(),
						gameEdge->getDelaunayEdge().second);
			}
			if(gameEdge->getVoronoiEdge().second){
				addToCenterList(gameEdge->getVoronoiEdge().second->getTouches(),
						gameEdge->getDelaunayEdge().first);
				addToCenterList(gameEdge->getVoronoiEdge().second->getTouches(),
						gameEdge->getDelaunayEdge().second);
			}
		}

		// CREATE POLYGONS FOR CELLS(CENTERs)
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			int s = iter->second->getPolyEdges().size();
			if(s <= 0) continue;

			iter->second->getPolyShape().setPointCount(s);

			CellEdge* e = iter->second->getPolyEdges()[0].get();
			sf::Vector2f current = e->getVoronoiEdge().first->getPoint();
			sf::Vector2f nextPoint = e->getVoronoiEdge().second->getPoint();
			std::vector<CellEdge*> visited;

			// walk corner points and construct polygon
			for(int i = 0; i < iter->second->getPolyEdges().size(); ++i){
				if(!e){
					break;
				}
				if(i > 0){
					if(Center::checkEdgePoint(e->getVoronoiEdge().first->getPoint(), nextPoint)){
						current = e->getVoronoiEdge().first->getPoint();
						nextPoint = e->getVoronoiEdge().second->getPoint();
					}else{
						current = e->getVoronoiEdge().second->getPoint();
						nextPoint = e->getVoronoiEdge().first->getPoint();
					}
				}

				iter->second->getPolyShape().setPoint(i, current);

				visited.push_back(e);
				e = iter->second->getNextVorEdge(&visited, nextPoint);
			}
			iter->second->setColour(sf::Color(51, 102, 0));
		}

		generateWater();
		// CREATE ELEVATION
		generateElevation();
		assignPolyColours();
	}

	void VoronoiMap::generateWater(){
		// center is outside island shape - mark as water
		for(auto iter = corners.begin(); iter != corners.end(); ++iter){
			if(!insideLandShape(iter->second->getPoint())){
				iter->second->setIsWater(true);
			}
			if(iter->second->isMapBorder()){
				iter->second->setElevation(0.0);
			}
		}

		// create water centers - additional ocean + lakes
		int numWater;
		float lakeThreshold = 0.1;
		std::queue<Center*> front;

		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			numWater = 0;
			for(auto c : iter->second->getCorners()){
				// mark border + ocean
				if(c->isMapBorder()){
					iter->second->setIsOcean(true);
					iter->second->setIsBorder(true);
					c->setIsWater(true);
					front.push(iter->second.get());
				}
				if(c->isWater()){
					numWater += 1;
				}
			}
			// mark lakes
			bool flag = (iter->second->isWater() ||
					numWater >= iter->second->getCorners().size() * lakeThreshold);
			iter->second->setIsWater(flag);

//			iter->second->setRainfall();
		}

		while(!front.empty()){
			Center* current = front.front();
			front.pop();

			for(auto n : current->getNeighbours()){
				if(n->isWater() && !n->isOcean()){
					n->setIsOcean(true);
					front.push(n.get());
				}
			}
		}

		// mark coastline
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			int numOcean = 0;
			int numLand = 0;

			for(auto n : iter->second->getNeighbours()){
				numOcean += (n->isOcean() ? 1 : 0);
				numLand += (!n->isWater() ? 1 : 0);
			}
			bool flag = (numOcean > 0) && (numLand > 0);
			iter->second->setIsCoast(flag);
		}

		for(auto iter = corners.begin(); iter != corners.end(); ++iter){
			int numOcean = 0;
			int numLand = 0;
			for(auto t : iter->second->getTouches()){
				numOcean += (t->isOcean() ? 1 : 0);
				numLand += (!t->isWater() ? 1 : 0);
			}
			bool flag = (numOcean >= iter->second->getTouches().size());
			iter->second->setIsOcean(flag);

			flag = (numOcean > 0) && (numLand > 0);
			iter->second->setIsCoast(flag);

			flag = iter->second->isMapBorder() ||
					((numLand != iter->second->getTouches().size()) &&
							!iter->second->isCoast());
			iter->second->setIsWater(flag);
		}

	}

	void VoronoiMap::generateElevation(){
		// 	use perlin noise to generate height map
		FastNoise perlinNoise;
		perlinNoise.SetSeed(gen());
		perlinNoise.SetNoiseType(FastNoise::Perlin);
		perlinNoise.SetInterp(FastNoise::Linear);
		// decrease to create larger patches of similar height
		// increase for more extreme, rugged patches of mountains
		perlinNoise.SetFrequency(0.0070);

		//	set poly/center elevation
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			if(iter->second->isWater()){
				iter->second->setElevation(0.0);
			}else{
				// elevFactor - increase to increase general elevation,
				// decrease to produce flatter land
				float elevFactor = 0.9;
				float e = (elevFactor + perlinNoise.GetNoise(iter->second->getPoint().x, iter->second->getPoint().y)) / 2;
				iter->second->setElevation(e);
			}
		}
		//	set corner elevations to the avg of neighbouring poly/centers
		for(auto iter = corners.begin(); iter != corners.end(); ++iter){
			float e = 0;
			for(auto c : iter->second->getTouches()){
				e += c->getElevation();
			}
			e = e / iter->second->getTouches().size();
			iter->second->setElevation(e);
		}

		// fill sinks - erodes terrain to make rivers look more natural
		// Ref: (function fillSinks(h, epsilon)) https://github.com/mewo2/terrain/blob/master/terrain.js
		/*
		 *  There's an obvious problem when we reach gridpoints which are lower than all
		 *  of their neighbours. Do we route the water back uphill? This will probably
		 *  lead to cycles in the water system, which are trouble. Instead, we want to
		 *  fill in these gaps (often called sinks or depressions), so that the water
		 *  always runs downhill all the way to the edge.
		 */
		std::map<std::pair<float, float>, float> newHeightmap;
		std::queue<CellCorner*> front;

		for(auto iter = corners.begin(); iter != corners.end(); ++iter){
			if(iter->second->isMapBorder()){
				newHeightmap[{iter->second->getPoint().x, iter->second->getPoint().y}]
							 = 0;
				front.push(iter->second.get());
			}else{
				newHeightmap[{iter->second->getPoint().x, iter->second->getPoint().y}]
							 = 9999;
			}
		}

		float epsilon = 0.0125;
		while(!front.empty()){
			CellCorner* c = front.front();
			front.pop();

			for(auto n : c->getAdjacent()){
				if(c->getElevation() >=
						newHeightmap[{n->getPoint().x, n->getPoint().y}] + epsilon){
					newHeightmap[{c->getPoint().x, c->getPoint().y}] = c->getElevation();
					front.push(n.get());
					break;
				}
				float k = newHeightmap[{n->getPoint().x, n->getPoint().y}] + epsilon;
				if((newHeightmap[{n->getPoint().x, n->getPoint().y}] > k) &&
						(k > c->getElevation())){
					newHeightmap[{c->getPoint().x, c->getPoint().y}] = k;
					front.push(n.get());
				}
			}
		}

		// create list of non-water tiles
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			if(!iter->second->isWater() && iter->second->getElevation() > 0.0){
				landCenters[{iter->second->getPoint().x,
					iter->second->getPoint().y}] = iter->second;
			}
		}

		// river generation
		/*
		 * 	delaunay triangle is a voronoi corner point surrounded by a triangle made
		 * 	up from voronoi center points. Corner point contains links to delaunay edges/ centers
		 */
		std::queue<CellCorner*> delTriFront;

		std::map<CellCorner*, CellCorner*> outflow;		// del tri paths
		std::map<CellCorner*, CellCorner*> watershed;	// del tri

		// Establish corner queue front
		for(auto iter = corners.begin(); iter != corners.end(); ++iter){
			bool nextToOcean = false;
			for(auto n : iter->second->getAdjacent()){
				if(n->isOcean()){
					nextToOcean = true;
				}
			}
			if(!iter->second->isMapBorder() && !iter->second->isOcean()){
				delTriFront.push(iter->second.get());
				// init delaunay maps
				watershed[iter->second.get()] = iter->second.get();
				outflow[iter->second.get()] = iter->second.get();
			}
		}

		while(!delTriFront.empty()){
			CellCorner* c = delTriFront.front();
			delTriFront.pop();

			int delTriangleSides = 2;
			std::uniform_int_distribution<> randN(0, delTriangleSides);
			int randIndex = randN(gen);

			for(int i = 0; i <= delTriangleSides; ++i){
				CellCorner* n = c->getAdjacent()[(i + randIndex) % delTriangleSides].get();
				if(outflow[n] == nullptr){
					outflow[n] = c;
					watershed[n] = watershed[c];
					if(!n->isWater() && !n->isOcean()){
						delTriFront.push(n);
					}
				}
			}
		}

//		for(int i = 0; i < polyFront.size(); ++i){
//			int pivot = std::ceil((polyFront.size() - i) / 2);
//
//			// infinite loop here - looping due to constant swapping?
//			for(int k = polyFront.size() - 1; k > i; k -= pivot){
//				float a = polyFront[k]->getElevation();
//				float b = polyFront[i]->getElevation();
//
//				if(a < b){
//					Center* s = polyFront[k];
//					polyFront[k] = polyFront[i];
//					polyFront[i] = s;
//				}
//			}
//
//			Center* c = polyFront[i];
//
//			for(auto n : c->getNeighbours()){
//				if((outflow.find(n.get()) == outflow.end()) || !outflow[n.get()]){
//					outflow[n.get()] = c;
//					watershed[n.get()] = watershed[c];
//					Center* n2 = n.get();
//					if(!n->isWater() && n->getElevation() > 0.0 &&
//							(std::find(polyFront.begin(), polyFront.end(),
//									n.get()) == polyFront.end())){
//						polyFront.push_back(n.get());
//					}
//				}
//			}
//		}

		for(auto iter = corners.begin(); iter != corners.end(); ++iter){
			CellCorner* downStream = outflow[iter->second.get()];

			if(downStream){
				CellEdge* e = iter->second->getEdge(downStream);
				e->setRiver(2);
			}
		}

		//	calc downslopes: At every corner point, we point to the
	    // point downstream from it, or to itself.  This is used for
	    // generating rivers and watersheds.+
//		for(auto iter = corners.begin(); iter != corners.end(); ++iter){
//			iter->second->setDownSlope(iter->second);
//			for(auto c : iter->second->getAdjacent()){
//				if(c->getElevation() <= iter->second->getElevation()){
//					iter->second->setDownSlope(c);
//				}
//			}
//		}

		// USING AMITS DOWNSLOPES AND RIVER CODE WONT WORK BECAUSE ITS
		//	INCOMPATIBLE WITH PERLIN NOISE HEIGHT. IT MUST BE USED WITH AN ISLAND
		// THAT HAS ELEVATIONS SLOPING DOWN TOWARDS THE COAST
//		int riverChance = 100;
//		printf("riverChance: %d\n", riverChance);
//		int riverCount = 0;
//		std::uniform_int_distribution<> cornerRange(0, corners.size() - 1);
//		for(int i = 0; i < riverChance; ++i){
//			auto randIter = corners.begin();
//			std::advance(randIter, cornerRange(gen));
//			CellCorner* c = randIter->second.get();
//
//			// river conditions/requirements
//			if(c->isOcean() || c->getElevation() < 0.3 || c->getElevation() > 0.9){
//				continue;
//			}
//
//			riverCount += 1;
//			while(!c->isCoast()){
//				if(c == c->getDownSlope().get()) break;
//
//				CellEdge* edge;
//				// get
//				for(auto e : c->getProtrudes()){
//					if(e->getVoronoiEdge().first.get() == c->getDownSlope().get() ||
//							e->getVoronoiEdge().second.get() == c->getDownSlope().get()){
//						edge = e.get();
//					}
//				}
//
//				edge->setRiver(edge->getRiver() + 1);
//				c->setRiver(c->getRiver() + 1);
//				c->getDownSlope()->setRiver(c->getDownSlope()->getRiver() + 1);
//				c = c->getDownSlope().get();
//			}
//		}
//
//		printf("riverCount: %d\n\n", riverCount);
	}
	void VoronoiMap::assignPolyColours(){
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
			if(iter->second->isCoast() && !iter->second->isWater() && iter->second->getElevation() <= 0.50){
				iter->second->setColour(sf::Color(204, 204, 0));
			}else if(iter->second->getElevation() >= 0.70){
				iter->second->setColour(sf::Color(200, 200, 200));
			}else if(iter->second->getElevation() >= 0.56){
				iter->second->setColour(sf::Color(77, 77, 77));
			}else if(iter->second->getElevation() <= 0.15 && !iter->second->isCoast()
					&& !iter->second->isOcean()){
				iter->second->setIsWater(true);
				for(auto c : iter->second->getCorners()){
					c->setIsWater(true);
				}
				iter->second->setColour(sf::Color(51, 133, 255));
			}
		}
	}

	void VoronoiMap::toggleElevationDraw(){
		if(!drawElevation){
			for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
				if(iter->second->getElevation() < 0.0){
					sf::Color colour(255, 0, 0);
					iter->second->getPolyShape().setFillColor(colour);
				}else if(iter->second->getElevation() == 0.0){
					sf::Color colour(0, 0, 0);
					iter->second->getPolyShape().setFillColor(colour);
				}else if(iter->second->getElevation() <= 0.2){
					sf::Color colour(50, 50, 50);
					iter->second->getPolyShape().setFillColor(colour);
				}else if(iter->second->getElevation() <= 0.4){
					sf::Color colour(100, 100, 100);
					iter->second->getPolyShape().setFillColor(colour);
				}else if(iter->second->getElevation() <= 0.6){
					sf::Color colour(165, 165, 165);
					iter->second->getPolyShape().setFillColor(colour);
				}else if(iter->second->getElevation() <= 0.8){
					sf::Color colour(200, 200, 200);
					iter->second->getPolyShape().setFillColor(colour);
				}else if(iter->second->getElevation() <= 1.0){
					sf::Color colour(255, 255, 255);
					iter->second->getPolyShape().setFillColor(colour);
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

	void VoronoiMap::toggleKingdomDraw(){
		if(!drawKingdoms){
			for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
				switch(iter->second->getOwner()){
				case KingdomUtil::KingdomType::HUMAN:
				{
					sf::Color orange(255, 179, 102);
					iter->second->getPolyShape().setFillColor(orange);
					break;
				}
				case KingdomUtil::KingdomType::DWARF:
				{
					sf::Color blue(102, 163, 255);
					iter->second->getPolyShape().setFillColor(blue);
					break;
				}
				case KingdomUtil::KingdomType::ELF:
				{
					sf::Color green(77, 255, 77);
					iter->second->getPolyShape().setFillColor(green);
					break;
				}
				case KingdomUtil::KingdomType::REPUB:
				{
					sf::Color red(255, 102, 102);
					iter->second->getPolyShape().setFillColor(red);
					break;
				}
				}
			}
			drawKingdoms = true;
		}else{
			for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){
				iter->second->reApplyColour();
			}
			drawKingdoms = false;
		}
	}

	void VoronoiMap::draw(sf::RenderWindow* window){
		for(auto iter = allCenters.begin(); iter != allCenters.end(); ++iter){

			window->draw(iter->second->getPolyShape());

			sf::RectangleShape point(sf::Vector2f(4, 4));
			point.setPosition(iter->second->getPoint());
			point.setFillColor(sf::Color::White);
			window->draw(point);
		}
		for(auto e : edges){
			if(!e->isRiver()) continue;

			window->draw(*e->getVorLine());
		}

		if(drawElevation){
			for(auto iter = corners.begin(); iter != corners.end(); ++iter){
				CellCorner* a = iter->second->getDownslopeEdge()->getVoronoiEdge().first.get();
				CellCorner* b = iter->second->getDownslopeEdge()->getVoronoiEdge().second.get();

				if(b->getElevation() > a->getElevation()){
					CellCorner* t = a;
					a = b;
					b = t;
				}
				float e = (a->getElevation() + b->getElevation()) / 2;

				sf::LineShape lineDown(a->getPoint(), b->getPoint());
				lineDown.setThickness(3);
				if(e < 0.3){
					lineDown.setFillColor(sf::Color(210, 255, 210));
				}else if(e < 0.35){
					lineDown.setFillColor(sf::Color(180, 255, 180));
				}else if(e < 0.4){
					lineDown.setFillColor(sf::Color(150, 255, 150));
				}else if(e < 0.45){
					lineDown.setFillColor(sf::Color(130, 255, 130));
				}else if(e < 0.5){
					lineDown.setFillColor(sf::Color(105, 255, 105));
				}else if(e < 0.55){
					lineDown.setFillColor(sf::Color(75, 255, 75));
				}else if(e < 0.6){
					lineDown.setFillColor(sf::Color(51, 255, 51));
				}else if(e < 0.65){
					lineDown.setFillColor(sf::Color(26, 255, 26));
				}else if(e < 0.7){
					lineDown.setFillColor(sf::Color(0, 255, 0));
				}else if(e < 0.75){
					lineDown.setFillColor(sf::Color(0, 235, 0));
				}else if(e < 0.8){
					lineDown.setFillColor(sf::Color(0, 205, 0));
				}else if(e < 0.85){
					lineDown.setFillColor(sf::Color(0, 180, 0));
				}else if(e < 0.9){
					lineDown.setFillColor(sf::Color(0, 150, 0));
				}else if(e < 0.95){
					lineDown.setFillColor(sf::Color(0, 135, 0));
				}else if(e < 2.0){
					lineDown.setFillColor(sf::Color(0, 120, 0));
				}

				window->draw(lineDown);

				float pointSize = 10 * iter->second->getElevation();
				sf::RectangleShape point(sf::Vector2f(pointSize, pointSize));
				point.setPosition(iter->second->getPoint());
				point.setFillColor(sf::Color::Yellow);
				window->draw(point);
			}
		}
	}
	void VoronoiMap::mouseMoved(float x, float y){
		if(x >= 0 && y >= 0 && x <= mapW && y <= mapH){
			Center* c = getClosestCell(x, y);
			if(c && c != mouseOver){
				if(mouseOver) mouseOver->reApplyColour();

				c->getPolyShape().setFillColor(highlightColour);
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
				Center* dEdgePoint = e->getDelaunayEdge().first.get();
				if(dEdgePoint->getPoint() == current->getPoint() || !dEdgePoint){
					dEdgePoint = e->getDelaunayEdge().second.get();
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
	CornerPair VoronoiMap::createVoronoiEdge(Edge* e){
		CornerPair vEdge;

		if(e->vertA){
			sf::Vector2f v1Point(e->vertA->x, e->vertA->y);
			vEdge.first = createCorner(v1Point);
		}else{
			vEdge.first = nullptr;
		}
		if(e->vertB){
			sf::Vector2f v2Point(e->vertB->x, e->vertB->y);
			vEdge.second = createCorner(v2Point);
		}else{
			vEdge.second = nullptr;
		}
		return vEdge;
	}
	void VoronoiMap::reset(){
		corners.clear();
		edges.clear();
		allCenters.clear();
		lastCenter = nullptr;
		mouseOver = nullptr;
	}
	CenterPair VoronoiMap::createDelaunayEdge(Edge* e){
		CenterPair dEdge;
		if(e->lSite){
			dEdge.first = allCenters[{e->lSite->p.x, e->lSite->p.y}];
		}else{
			dEdge.first = nullptr;
		}
		if(e->rSite){
			dEdge.second = allCenters[{e->rSite->p.x, e->rSite->p.y}];
		}else{
			dEdge.second = nullptr;
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
}
