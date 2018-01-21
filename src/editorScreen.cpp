/*
 * editorScreen.cpp
 *
 *  Created on: 13 Mar 2016
 *      Author: linbob
 */

#include "main.hpp"
#include "editorScreen.hpp"

void EditorScreen::init(){
	loadResources();

	status = RUNNING;

	//WORLD CAMERA + MOVEMENT LISTENER
	worldView = std::make_shared<WorldView>(SCREEN_WIDTH, SCREEN_HEIGHT, input);

	entityWorld = std::make_shared<anax::World>();

	//ENTITY FACTORY
	entFactory = std::make_shared<EntityFactory>("bin/data/entityData.json",
			entityWorld.get(), atlas.get());

	//MAP
	map = std::make_shared<TileMap>(entFactory.get(), "bin/data/tileData.json", MAP_W, MAP_W, atlas);

	createDefaultMap();

	worldView->view->setCenter(2 * TILESIZE, (map->getHeight() - 4) * TILESIZE);

	//PLACEMENT MANAGER
	placement = std::make_shared<PlacementManager>(worldView.get(), manager->getWindow());
	placement->addPlaceable("tile", std::unique_ptr<Placeable>(
			new TilePlaceable(map.get(), worldView.get(), manager->getWindow())));
	placement->addPlaceable("entity", std::unique_ptr<Placeable>(
			new EntityPlaceable(entFactory.get(), map.get(), worldView.get(),
					manager->getWindow())));

	areaEdit = std::make_shared<AreaEdit>(worldView.get(), entFactory.get(), map.get());

	//CONTROL AREA
	EditorCtrlArea::createCtrlArea(this);
	EditorCtrlArea::loadTileButtons(this);
	EditorCtrlArea::loadEntityButtons(this);

	TooltipAnchor::Ptr tt = std::make_shared<TooltipAnchor>(map.get(), entityWorld.get(),
			worldView.get(), manager);
	manager->getGui()->add(tt, "tooltip");

	//Input Actions
	input.getMap()["click_place"] = thor::Action(sf::Mouse::Left, thor::Action::PressOnce);

	input.getActionSys().connect("click_place", std::bind([this](){
		if(placement->leftClick(worldView.get(), manager->getWindow())) return;
		if(areaEdit->leftClick(worldView.get(), manager->getWindow())) return;
	}));

	input.getMap()["drag_place"] = thor::Action(sf::Event::MouseMoved) &&
			thor::Action(sf::Mouse::Left, thor::Action::Hold);

	input.getActionSys().connect("drag_place", std::bind([this](){
		placement->mouseMoved(worldView.get(), manager->getWindow());
	}));

	input.getMap()["reset_place"] = thor::Action(sf::Mouse::Right, thor::Action::PressOnce);

	input.getActionSys().connect("reset_place", std::bind([this](){
		placement->rightClick();
		areaEdit->rightClick();
	}));

	cursorOutline = std::make_shared<CursorOutline>(CursorOutline(map.get(), &input));
	cursorOutline->setCurrent("normal");

	input.getMap()["show_tooltip"] = thor::Action(sf::Event::MouseMoved);

	input.getActionSys().connect("show_tooltip", std::bind([tt, this]{
		tt->mouseMoved();
		cursorOutline->mouseMoved(worldView.get(), manager->getWindow());
	}));

}
void EditorScreen::createDefaultMap(){
	//CREATE DEBUG/TEST MAP
	for(int x = 0; x < map->getWidth(); ++x){
		for(int y = 0; y < map->getHeight(); ++y){
			if(x == 0 || y == 0 || x == map->getWidth() - 1 || y == map->getHeight() - 1){
				map->getTileLayer().setTile(x, y, map->getFactory()->createTile(20));
				map->getTileLayer().getTile(x, y)[0]->setPosition(x * TILESIZE, y * TILESIZE);
				map->getTileLayer().setTile(x, y, map->getFactory()->createTile(101));
				map->getTileLayer().getTile(x, y)[1]->setPosition(x * TILESIZE, y * TILESIZE);
			}else{
				map->getTileLayer().setTile(x, y, map->getFactory()->createTile(16));
				map->getTileLayer().getTile(x, y)[0]->setPosition(x * TILESIZE, y * TILESIZE);
			}
		}
	}
}
void EditorScreen::load(std::string path){
	if(status == UNINIT){
		Screen::run();
		init();
	}
	map->load(path, entFactory.get());
}
void EditorScreen::save(std::string path){
	map->save(path);
}
int EditorScreen::run(){
	Screen::run();

	if(status == UNINIT){
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
void EditorScreen::update(sf::Time& delta){
	entityWorld->refresh();

	manager->getGui()->updateTime(delta);
}
void EditorScreen::render(sf::Time& delta){
	manager->getWindow()->clear(sf::Color::Black);

	//draw world
	manager->getWindow()->setView(*worldView->view);

	map->render(manager->getWindow());

	placement->render(*manager->getWindow());

	areaEdit->render(*manager->getWindow());

	cursorOutline->render(*manager->getWindow());

	manager->getGui()->draw();

	// end the current frame
	manager->getWindow()->display();
}
void EditorScreen::handleInput(sf::Time& delta){
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
			if(!manager->getGui()->handleEvent( event )){
				input.getMap().pushEvent(event);
			}
			break;
		}
	}
	input.getMap().invokeCallbacks(input.getActionSys(), manager->getWindow());
}
void EditorScreen::resize(sf::Event& e){
	worldView->resize(e.size.width, e.size.height);
}
void EditorScreen::loadResources(){
	atlas = std::make_shared<TextureAtlas>("bin/imgs/", "atlas", 3);
}


