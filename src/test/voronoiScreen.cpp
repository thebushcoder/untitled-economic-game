/*
 * voronoiScreen.cpp
 *
 *  Created on: 10 Dec 2017
 *      Author: linbob
 */

#include "voronoiScreen.hpp"

void VoronoiScreen::init(){
	status = RUNNING;

	int numSites = 600, w = 1280, h = 1024;

	voronoiDiagram = new VoronoiMap::VoronoiMap(numSites, w, h);

	VoronoiMap::TerrainGeneration::generateWater(voronoiDiagram);
	VoronoiMap::TerrainGeneration::generateElevation(voronoiDiagram);
	VoronoiMap::TerrainGeneration::generateRivers(voronoiDiagram);
	VoronoiMap::TerrainGeneration::generateMoisture(voronoiDiagram);
	voronoiDiagram->assignPolyColours();

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

	KingdomUtil::KingdomUtil::genKingdomArea(kingdoms, voronoiDiagram);

	input.getMap()["relax_diagram"] = thor::Action(sf::Keyboard::F1, thor::Action::PressOnce);
	input.getActionSys().connect("relax_diagram", std::bind([this]{
		voronoiDiagram->relaxDiagram();
		VoronoiMap::TerrainGeneration::generateWater(voronoiDiagram);
		VoronoiMap::TerrainGeneration::generateElevation(voronoiDiagram);
		VoronoiMap::TerrainGeneration::generateRivers(voronoiDiagram);
		VoronoiMap::TerrainGeneration::generateMoisture(voronoiDiagram);
		voronoiDiagram->assignPolyColours();
		KingdomUtil::KingdomUtil::genKingdomArea(kingdoms, voronoiDiagram);
	}));

	input.getMap()["gen_diagram"] = thor::Action(sf::Keyboard::F2, thor::Action::PressOnce);
	input.getActionSys().connect("gen_diagram", std::bind([numSites, this]{
		voronoiDiagram->reset();
		voronoiDiagram->generateNewMap(numSites);
		VoronoiMap::TerrainGeneration::generateWater(voronoiDiagram);
		VoronoiMap::TerrainGeneration::generateElevation(voronoiDiagram);
		VoronoiMap::TerrainGeneration::generateRivers(voronoiDiagram);
		VoronoiMap::TerrainGeneration::generateMoisture(voronoiDiagram);
		voronoiDiagram->assignPolyColours();
		KingdomUtil::KingdomUtil::genKingdomArea(kingdoms, voronoiDiagram);
	}));

	input.getMap()["toggle_kingdoms"] = thor::Action(sf::Keyboard::F3, thor::Action::PressOnce);
	input.getActionSys().connect("toggle_kingdoms", std::bind([this]{
		voronoiDiagram->toggleKingdomDraw();
	}));

	input.getMap()["toggle_elevation"] = thor::Action(sf::Keyboard::F4, thor::Action::PressOnce);
	input.getActionSys().connect("toggle_elevation", std::bind([this]{
		voronoiDiagram->toggleElevationDraw();
	}));

	input.getMap()["toggle_moisture"] = thor::Action(sf::Keyboard::F5, thor::Action::PressOnce);
	input.getActionSys().connect("toggle_moisture", std::bind([this]{
		voronoiDiagram->toggleMoistureDraw();
	}));

	input.getMap()["mouse_moved"] = thor::Action(sf::Event::MouseMoved);
	input.getActionSys().connect("mouse_moved", std::bind([this]{
		sf::Vector2i p = sf::Mouse::getPosition(*manager->getWindow());

		voronoiDiagram->mouseMoved(p.x, p.y);
	}));

	input.getMap()["mouse_clicked"] = thor::Action(sf::Mouse::Left, thor::Action::PressOnce);
	input.getActionSys().connect("mouse_clicked", std::bind([this]{
		sf::Vector2i p = sf::Mouse::getPosition(*manager->getWindow());

		voronoiDiagram->mouseClick(p.x, p.y);
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


	voronoiDiagram->draw(manager->getWindow());

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
