/*
 * mainMenu.cpp
 *
 *  Created on: 19 Feb 2016
 *      Author: linbob
 */

#include "mainMenu.hpp"

void MainMenu::init(){
	status = RUNNING;

	createMainControls();
	createGameButtons();
}
void MainMenu::createMainControls(){
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	auto mainLayout = std::make_shared<tgui::VerticalLayout>();
	mainLayout->setSize(400.0f, manager->getWindow()->getSize().y * 0.4);
	mainLayout->setPosition(windowWidth * 0.35,	windowHeight * 0.3);

	tgui::Button::Ptr button = manager->getTheme()->load("Button");
	button->setText("Start");

	button->connect("pressed", [this]{
		manager->getGui()->get("main")->hide();
		manager->getGui()->get("game")->show();
	});

	// Add the Button to the Box
	mainLayout->add(button);
	mainLayout->addSpace(0.2f);

	button = manager->getTheme()->load("Button");
	button->setText("Editor");

	button->connect("pressed", [this]{
		status = QUIT;
		Screen* s = new EditorScreen();
		s->setManager(manager);
		manager->setCurrentScreen(s);
	});
	// Add the Button to the Box
	mainLayout->add(button);
	mainLayout->addSpace(0.2f);

	button = manager->getTheme()->load("Button");
	button->setText("Options");
	button->connect("pressed", [this]{
		status = QUIT;
		Screen* s = new VoronoiScreen();
		s->setManager(manager);
		manager->setCurrentScreen(s);
	});
	// Add the Button to the Box
	mainLayout->add(button);
	mainLayout->addSpace(0.2f);

	button = manager->getTheme()->load("Button");
	button->setText("Exit");
	// Add the Button to the Box
	mainLayout->add(button);
	button->connect("pressed", [this]{
		status = QUIT;
	});
	manager->getGui()->add(mainLayout, "main");
}

void MainMenu::createGameButtons(){
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	auto gameLayout = std::make_shared<tgui::VerticalLayout>();
	gameLayout->setSize(400.0f, manager->getWindow()->getSize().y * 0.4);
	gameLayout->setPosition(windowWidth * 0.35,	windowHeight * 0.3);

	tgui::Button::Ptr button = manager->getTheme()->load("Button");
	button->setText("Select Map");

	button->connect("pressed", [this]{
//		status = QUIT;
//		manager->setCurrentScreen("game");
//
//		GameScreen* game = (GameScreen*)getManager()->getCurrentScreen();
//		game->load("bin/mapTest.save");
//		showScreenWidgets(false);
	});

	// Add the Button to the Box
	gameLayout->add(button);
	gameLayout->addSpace(0.2f);

	SaveFileWindow::Ptr loadWindow = std::make_shared<SaveFileWindow>();
	loadWindow->init(manager->getTheme(), "game/", true);

	loadWindow->get("load")->disconnectAll();
	loadWindow->get("load")->connect("pressed", [loadWindow, this]{
		tgui::ListBox::Ptr list = std::dynamic_pointer_cast<tgui::ListBox>(loadWindow->get("file_list"));
		if(list->getSelectedItem().isEmpty()) return;

		GameScreen* s = new GameScreen();
		s->setManager(manager);

		status = QUIT;
		manager->setCurrentScreen(s);

		s->load(loadWindow->rootPath + loadWindow->instancePath + list->getSelectedItem());
	});
	manager->getGui()->add(loadWindow, "loadWindow");

	button = manager->getTheme()->load("Button");
	button->setText("Load Game");
	button->connect("pressed", [loadWindow]{
		loadWindow->show();
		loadWindow->moveToFront();
	});
	// Add the Button to the Box
	gameLayout->add(button);
	gameLayout->addSpace(0.2f);

	button = manager->getTheme()->load("Button");
	button->setText("Back");

	button->connect("pressed", [this]{
		manager->getGui()->get("main")->show();
		manager->getGui()->get("game")->hide();
	});

	// Add the Button to the Box
	gameLayout->add(button);

	gameLayout->hide();
	manager->getGui()->add(gameLayout, "game");
}
int MainMenu::run(){
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

		render();
	}

	return QUIT;
}
void MainMenu::update(sf::Time& delta){
	manager->getGui()->updateTime(delta);
}
void MainMenu::render(){
	manager->getWindow()->clear(sf::Color::Black);

	manager->getGui()->draw();

	// end the current frame
	manager->getWindow()->display();
}
void MainMenu::handleInput(sf::Time& delta){
	sf::Event event;

	while (manager->getWindow()->pollEvent(event)){
		// "close requested" event: we close the window
		switch (event.type){
		case sf::Event::Closed:
			status = QUIT;
			getManager()->setCurrentScreen(nullptr);
			break;
		case sf::Event::Resized:
			resize(event);
			break;
		default:
			manager->getGui()->handleEvent( event );
			break;
		}
	}
}
void MainMenu::resize(sf::Event& e){
	manager->getWindow()->setView(sf::View(sf::FloatRect(0, 0,
			e.size.width, e.size.height)));

	manager->getGui()->setView(manager->getWindow()->getView());
}
void MainMenu::loadResources(){}
