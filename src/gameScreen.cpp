/*
 * gameScreen.cpp
 *
 *  Created on: 20 Feb 2016
 *      Author: linbob
 */

#include "main.hpp"
#include "gameScreen.hpp"
#include "libado/util/bt-master/btLuaBind.hpp"

void GameScreen::init(){
	loadResources();

	status = RUNNING;
	entityWorld = std::make_shared<anax::World>();

	//ENTITY FACTORY
	entFactory = std::make_shared<EntityFactory>("bin/data/entityData.json",
			entityWorld.get(), atlas.get());

	//CENTRE CAM ON PLAYER
//	worldView->view->setCenter(0, 0);

	overworld = std::unique_ptr<Overworld>(new Overworld(manager->getWindow()->getSize().x,
			manager->getWindow()->getSize().y, "overworld", getInput()));

	overworld->initTileMap(new TileMap(entFactory.get(), "bin/data/tileData.json", 32, 32, atlas));

	aiSys = std::make_shared<AiSystem>(overworld.get(), entFactory.get(), manager->getGui());

	entityWorld->addSystem(*aiSys);
	entityWorld->addSystem(*TownResourceSys::getInstance());

	lootManager = std::make_shared<LootListManager>(entFactory.get());

	/////////////////////////////////////////////////////////////

	//GUI STUFF
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	auto contextMenu = std::make_shared<PlayerContext>(this);
	manager->getGui()->add(contextMenu, "playerContext");

	LootWindow::Ptr lootWindow = manager->getTheme()->load("LootWindow");
	manager->getGui()->add(lootWindow, "loot_window");
	lootWindow->init(this);

	ItemInfoWindow::Ptr itemInfo = manager->getTheme()->load("ItemInfoWindow");
	itemInfo->init(manager->getTheme());
	manager->getGui()->add(itemInfo, "item_info_window");

	InventoryWindow::Ptr invenWindow = manager->getTheme()->load("InventoryWindow");
	invenWindow->setSize(654, 600);
	invenWindow->init(this);
	manager->getGui()->add(invenWindow, "inventory_window");

	ShopWindow::Ptr shopWindow = manager->getTheme()->load("ShopWindow");
	shopWindow->init(this);
	manager->getGui()->add(shopWindow, "shop_window");

	MountInvenWindow::Ptr mountInvenWin = manager->getTheme()->load("MountInvenWindow");
	mountInvenWin->init(this);
	manager->getGui()->add(mountInvenWin, "mount_window");

	TownResourceWindow::Ptr townResWin = manager->getTheme()->load("TownResWin");
	townResWin->init(TownResourceSys::getInstance(), manager->getTheme());
	manager->getGui()->add(townResWin, "townResWin");

	TownTooltip::Ptr tt = std::make_shared<TownTooltip>(overworld->zone.get(), entityWorld.get(),
				overworld->zoneView.get(), manager);
	manager->getGui()->add(tt, "town-tooltip");

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	GameCtrlArea::createCtrlArea(this);

	tgui::Panel::Ptr timeWidget = TimeWidget::createTimeWidget(manager->getTheme());
	tgui::Panel::Ptr ctrlArea = manager->getGui()->get<tgui::Panel>("game_panel");
	timeWidget->setPosition(tgui::bindWidth(*manager->getGui()) - (timeWidget->getSize().x),
			tgui::bindHeight(*manager->getGui()) - (ctrlArea->getSize().y * 1.2));
	manager->getGui()->add(timeWidget, "timeWidget");
	TimeSystem* timeSys = TimeSystem::getInstance();
	timeSys->setWidget(timeWidget);

	/////////////////////////////////////////////////////////////
	lua["behaviourTree"]["createBT"]();

	/*
	 * input hookups
	 */
	input.getMap()["lua_reload"] = thor::Action(sf::Keyboard::F4, thor::Action::PressOnce);
	input.getActionSys().connect("lua_reload", std::bind([this]{
		lua["reload"]();
	}));

	input.getMap()["show_inventory"] = thor::Action(sf::Keyboard::I, thor::Action::PressOnce);
	input.getActionSys().connect("show_inventory", std::bind([invenWindow]{
		invenWindow->show();
		return true;
	}));

	input.getMap()["show_resWindow"] = thor::Action(sf::Keyboard::L, thor::Action::PressOnce);
	input.getActionSys().connect("show_resWindow", std::bind([townResWin]{
		if(!townResWin->isVisible()){
			townResWin->show();
		}else{
			townResWin->hide();
		}

		return true;
	}));

	//display context menu for loot, inven window or map context menu
	input.getMap()["right_click"] = thor::Action(sf::Mouse::Right, thor::Action::PressOnce);
	input.getActionSys().connect("right_click", std::bind([invenWindow, mountInvenWin, shopWindow, lootWindow, this]{
		sf::Vector2i p = sf::Mouse::getPosition(*manager->getWindow());
		sf::Vector2f worldVec = manager->getWindow()->mapPixelToCoords(p,
				*(overworld->getActiveZone()->zoneView->view));

		//tile position
		int x = std::round(worldVec.x / TILESIZE);
		int y = std::round(worldVec.y / TILESIZE);

		//check map bounds
		if(x < 0 || y < 0 || x >= overworld->getActiveZone()->zone->getWidth() ||
				y >= overworld->getActiveZone()->zone->getHeight()) return false;

		//SHOW LOOT WINDOW RIGHT CLICK MENU
		if(lootWindow->rightClick(p.x, p.y)) return true;

		//SHOW INVENTORY WINDOW RIGHT CLICK MENU
		if(invenWindow->rightClick(p.x, p.y)) return true;

		//SHOW SHOP WINDOW RIGHT CLICK MENU
		if(shopWindow->rightClick(p.x, p.y)) return true;

		//SHOW mountInvenWin WINDOW RIGHT CLICK MENU
		if(mountInvenWin->rightClick(p.x, p.y)) return true;

		//SHOW RIGHT CLICK MENU
		PlayerContext::Ptr w = manager->getGui()->get<PlayerContext>("playerContext");
		w->setPosition(p.x, p.y);
		w->show(x, y);

		return true;
	}));

	//move to click/ idle
	input.getMap()["left_click"] = thor::Action(sf::Mouse::Left, thor::Action::PressOnce);
	input.getActionSys().connect("left_click", std::bind([this]{
		tgui::Widget::Ptr pContext = manager->getGui()->get("playerContext");
		tgui::Widget::Ptr iContext = manager->getGui()->get("inventory_context");

		if(pContext->isVisible()){
			pContext->hide();
			return true;
		}

//		if(!this->overworld->isChildActive())	return false;

		sf::Vector2i p = sf::Mouse::getPosition(*manager->getWindow());
		sf::Vector2f worldVec = manager->getWindow()->mapPixelToCoords(p,
				*(overworld->getActiveZone()->zoneView->view));

		//tile position
		int x = std::round(worldVec.x / TILESIZE);
		int y = std::round(worldVec.y / TILESIZE);

		//check map bounds
		if(x < 0 || y < 0 || x >= overworld->getActiveZone()->zone->getWidth() ||
				y >= overworld->getActiveZone()->zone->getHeight()) return false;

		PositionComponent& pos = entFactory->getPlayer().getComponent<PositionComponent>();
		AiComponent& ai = entFactory->getPlayer().getComponent<AiComponent>();
		if(pos.tilePosition.x == x && pos.tilePosition.y == y){
			//IDLE PLAYER 1 TURN
			ai.board->SetBool("idleTurn", true);
			ai.board->SetBool("needsInput", false);
			return true;
		}else if(overworld->getActiveZone()->zone->getCost(x, y) < Tile::IMPASSABLE){
			//MOVE PLAYER TO LEFT CLICK LOCATION
			ai.board->SetString("current_task", "walk_to");
			ai.board->SetVector("tgt_tile", sf::Vector2i(x, y));
			ai.board->SetBool("needsInput", false);
			return true;
		}

		return false;
	}));
}

int GameScreen::run(){
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

//		printf("gui isFocused : %s\n", manager->getGui()->isFocused() ? "true" : "false");

//		sf::Time logicTime = deltaClock.getElapsedTime();

		render(deltaTime);

//		sf::Time renderTime = deltaClock.getElapsedTime();

//		printf("Logic Time : %d\n", logicTime.asMilliseconds());
//		printf("Render Time : %d\n", renderTime.asMilliseconds() - logicTime.asMilliseconds());
	}

	return QUIT;
}

void GameScreen::load(std::string path){
	if(status == UNINIT){
		Screen::run();
		init();
	}

	std::string overworldPath = getenv("HOME");
	overworldPath += "/linbobGame/game/overworld.save";

	char readBuffer[65536];
	rapidjson::Document d;

	FILE* fp = fopen(&overworldPath[0], "r");
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	d.ParseStream(is);

	fclose(fp);

	overworld->loadOverworld(d, entFactory.get());
	overworld->setPath(overworldPath);

	if(d.HasMember("TimeSys")){
		TimeSystem::getInstance()->load(d["TimeSys"]);
	}

	std::regex base_regex("overworld\\.save");
	std::smatch base_match;
	if (!(std::regex_search(path, base_match, base_regex) && base_match.size() > 0)){
		overworld->loadChildZone(path, entFactory.get());
		overworld->setChildActive();
	}else{
		overworld->setChildActive(false);
	}

//	manager->getGui()->get<InventoryWindow>("inventory_window")->assignEquipmentSlots(
//			entFactory->getPlayer().getComponent<EquipmentComponent>()
//	);
}
void GameScreen::save(std::string path){
	FILE* fp;

	rapidjson::Document d;
	d.SetObject();

	overworld->saveOverworld(d);

	TimeSystem::getInstance()->save(d);

	//WRITE JSON TO FILE
	if(path.empty()){
		fp = fopen(overworld->getFilePath().c_str(), "w");
	}else{
		fp = fopen(path.c_str(), "w");
	}

	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	d.Accept(writer);

	fclose(fp);
}
void GameScreen::update(sf::Time& delta){
	entityWorld->refresh();

	aiSys->process(delta);

	manager->getGui()->updateTime(delta);
}
void GameScreen::render(sf::Time& delta){
	manager->getWindow()->clear(sf::Color::Black);

	//draw world
	overworld->render(manager->getWindow());

	//draw debug path
//	AiComponent& playerAi = entFactory->getPlayer().getComponent<AiComponent>();
//	for(auto& n : playerAi.path){
//		sf::RectangleShape nodeTile;
//		nodeTile.setSize((sf::Vector2f(TILESIZE, TILESIZE)));
//		nodeTile.setPosition(n.x * TILESIZE - TILESIZE / 2,
//				n.y * TILESIZE - TILESIZE / 2);
//		manager->getWindow()->draw(nodeTile);
//	}

	manager->getGui()->draw();

	// end the current frame
	manager->getWindow()->display();
}
void GameScreen::handleInput(sf::Time& delta){
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

void GameScreen::resize(sf::Event& e){
	overworld->resize(e.size.width, e.size.height);

	manager->getWindow()->setView(sf::View(sf::FloatRect(0, 0,
			e.size.width, e.size.height)));

	manager->getGui()->setView(manager->getWindow()->getView());
}

void GameScreen::loadResources(){
	atlas = std::make_shared<TextureAtlas>("bin/imgs/", "atlas", 3);

	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::table);

	tgui::Container::Ptr w = getManager()->getGui()->getContainer();
	lua["gui_root"] = w.get();
	lua["gameScreen"] = this;
	lua["print"] = Logging::printToConsole;

	WidgetLuaBind::initBindings(lua);
	btLuaBind::initBindings(lua);

	lua.script_file("/home/linbob/linbobGame/script.lua");
}
