/*
 * voronoiScreen.cpp
 *
 *  Created on: 10 Dec 2017
 *      Author: linbob
 */

#include "voronoiScreen.hpp"

void VoronoiScreen::init(){
	status = RUNNING;

	int numSites = 1800, w = 1600, h = 1024;

	terrainVorDia = std::unique_ptr<VoronoiMap::VoronoiMap>(
			new VoronoiMap::VoronoiMap(numSites, w, h));

	VoronoiMap::NoisyEdges::getInstance()->generateNoisyEdges(terrainVorDia.get());
	VoronoiMap::NoisyEdges::getInstance()->generateNoisyPolys(terrainVorDia.get());
	VoronoiMap::TerrainGeneration::generateWater(terrainVorDia.get());
	VoronoiMap::TerrainGeneration::generateElevation(terrainVorDia.get());
	VoronoiMap::TerrainGeneration::generateRivers(terrainVorDia.get());
	VoronoiMap::TerrainGeneration::generateMoisture(terrainVorDia.get());
	VoronoiMap::TerrainGeneration::generateBiomes(terrainVorDia.get());

	entityWorld = std::make_shared<anax::World>();
	anax::Entity k = entityWorld->createEntity();
	k.addComponent<ProvincesComponent>();
	k.addComponent<KingdomComponent>(KingdomUtil::KingdomType::HUMAN);
	kingdoms[KingdomUtil::KingdomType::HUMAN] = k;

	k = entityWorld->createEntity();
	k.addComponent<KingdomComponent>(KingdomUtil::KingdomType::ELF);
	k.addComponent<ProvincesComponent>();
	kingdoms[KingdomUtil::KingdomType::ELF] = k;

	k = entityWorld->createEntity();
	k.addComponent<KingdomComponent>(KingdomUtil::KingdomType::DWARF);
	k.addComponent<ProvincesComponent>();
	kingdoms[KingdomUtil::KingdomType::DWARF] = k;

	k = entityWorld->createEntity();
	k.addComponent<KingdomComponent>(KingdomUtil::KingdomType::REPUB);
	k.addComponent<ProvincesComponent>();
	kingdoms[KingdomUtil::KingdomType::REPUB] = k;

	KingdomUtil::KingdomUtil::generateKingdomVoronoi(this);
	provinceVorDia->toggleNoisyEdges();

	KingdomUtil::KingdomUtil::genKingdomArea(kingdoms, provinceVorDia.get());
	provinceVorDia->assignKingdomColours();


	input.getMap()["gen_diagram"] = thor::Action(sf::Keyboard::F2, thor::Action::PressOnce);
	input.getActionSys().connect("gen_diagram", std::bind([numSites, this]{
		terrainVorDia->reset();
		provinceVorDia->reset();
		terrainVorDia->generateNewMap(terrainVorDia->generateCellPoints(numSites));

		VoronoiMap::NoisyEdges::getInstance()->generateNoisyEdges(terrainVorDia.get());
		VoronoiMap::NoisyEdges::getInstance()->generateNoisyPolys(terrainVorDia.get());
		VoronoiMap::TerrainGeneration::generateWater(terrainVorDia.get());
		VoronoiMap::TerrainGeneration::generateElevation(terrainVorDia.get());
		VoronoiMap::TerrainGeneration::generateRivers(terrainVorDia.get());
		VoronoiMap::TerrainGeneration::generateMoisture(terrainVorDia.get());
		VoronoiMap::TerrainGeneration::generateBiomes(terrainVorDia.get());

		KingdomUtil::KingdomUtil::generateKingdomVoronoi(this);
		provinceVorDia->toggleNoisyEdges();

		KingdomUtil::KingdomUtil::genKingdomArea(kingdoms, provinceVorDia.get());
		provinceVorDia->assignKingdomColours();
	}));

	input.getMap()["toggle_kingdoms"] = thor::Action(sf::Keyboard::F3, thor::Action::PressOnce);
	input.getActionSys().connect("toggle_kingdoms", std::bind([this]{
		provinceVorDia->toggleKingdomDraw();
	}));

	input.getMap()["toggle_elevation"] = thor::Action(sf::Keyboard::F4, thor::Action::PressOnce);
	input.getActionSys().connect("toggle_elevation", std::bind([this]{
		terrainVorDia->toggleElevationDraw();
	}));

	input.getMap()["toggle_moisture"] = thor::Action(sf::Keyboard::F5, thor::Action::PressOnce);
	input.getActionSys().connect("toggle_moisture", std::bind([this]{
		terrainVorDia->toggleMoistureDraw();
	}));

	input.getMap()["toggle_noisy"] = thor::Action(sf::Keyboard::F6, thor::Action::PressOnce);
	input.getActionSys().connect("toggle_noisy", std::bind([this]{
		terrainVorDia->toggleNoisyEdges();
	}));

	input.getMap()["mouse_moved"] = thor::Action(sf::Event::MouseMoved);
	input.getActionSys().connect("mouse_moved", std::bind([this]{
		sf::Vector2i p = sf::Mouse::getPosition(*manager->getWindow());

		provinceVorDia->mouseMoved(p.x, p.y);
	}));

	input.getMap()["mouse_clicked"] = thor::Action(sf::Mouse::Left, thor::Action::PressOnce);
	input.getActionSys().connect("mouse_clicked", std::bind([this]{
		sf::Vector2i p = sf::Mouse::getPosition(*manager->getWindow());

		terrainVorDia->mouseClick(p.x, p.y);
	}));
}
int VoronoiScreen::run(){
	if(status == UNINIT){
		Screen::run();
		init();
	}else{
		status = RUNNING;
	}

	sf::Clock deltaClock;
	// run the program as long as the window is open
	while(status == RUNNING){
		sf::Time deltaTime = deltaClock.restart();

		handleInput(deltaTime);

		update(deltaTime);

		render(deltaTime);
	}

	return QUIT;
}
void VoronoiScreen::update(sf::Time& delta){
	manager->getGui()->updateTime(delta);
}
void VoronoiScreen::render(sf::Time& delta){
	manager->getWindow()->clear(sf::Color::Black);


	terrainVorDia->draw(manager->getWindow());
	if(provinceVorDia->isDrawingKingdoms()){
		provinceVorDia->draw(manager->getWindow());
	}

	manager->getGui()->draw();

	// end the current frame
	manager->getWindow()->display();
}
void VoronoiScreen::handleInput(sf::Time& delta){
	sf::Event event;

	input.setDelta(delta);
	input.getMap().clearEvents();

	while (manager->getWindow()->pollEvent(event)){
		// "close requested" event: we close the window
		switch (event.type){
		case sf::Event::Closed:
			status = QUIT;
			getManager()->setCurrentScreen(nullptr);
			break;
		case sf::Event::Resized:
			resize(event);
			manager->resize(event);
			break;
		default:
			// if the gui doesnt handle the even pass it on to the ThorInput sys
			if(!manager->getGui()->handleEvent( event )){
				input.getMap().pushEvent(event);
			}

			break;
		}
	}
	input.getMap().invokeCallbacks(input.getActionSys(), manager->getWindow());
}
void VoronoiScreen::resize(sf::Event& e){
	manager->getWindow()->setView(sf::View(sf::FloatRect(0, 0,
			e.size.width, e.size.height)));

	manager->getGui()->setView(manager->getWindow()->getView());
}
void VoronoiScreen::loadResources(){}
