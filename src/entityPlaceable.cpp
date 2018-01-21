/*
 * entityPlacement.cpp
 *
 *  Created on: 29 Mar 2016
 *      Author: linbob
 */

#include "entityPlaceable.hpp"

EntityPlaceable::EntityPlaceable(EntityFactory* ef, TileMap* m, WorldView* v, sf::RenderWindow* w)
	:	map(m),factory(ef){}

bool EntityPlaceable::place(int tileX, int tileY){
	if(tileX < 0 || tileY < 0 || tileX >= map->getWidth() ||
			tileY >= map->getHeight()) return false;

	anax::Entity e = factory->createEntity(entityName);

	map->getEntityLayer().setEntity(tileX, tileY, e);

	//offset(centre) sprites larger than tile size
	sf::Vector2f offset;
	if(preview->getGlobalBounds().height > TILESIZE){
		offset.y = (preview->getGlobalBounds().height - TILESIZE) / 2;
	}
	if(preview->getGlobalBounds().width > TILESIZE){
		offset.x = (preview->getGlobalBounds().width - TILESIZE) / 2;
	}

	PositionComponent& pc = e.getComponent<PositionComponent>();
	pc.setPosition(tileX, tileY, tileX * TILESIZE - offset.x, tileY * TILESIZE - offset.y);

	if((e.getComponent<NameComponent>()).name.compare("blank-town") == 0){
		// dialog prompt for town name
		townNamePrompt(e);
	}

	if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
		reset();
	}
	return true;
}
void EntityPlaceable::render(sf::RenderWindow& window){
	sf::Vector2f converted = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	int x = std::round(converted.x / TILESIZE);
	int y = std::round(converted.y / TILESIZE);

	if(x < 0 || y < 0 || x >= map->getWidth() ||
			y >= map->getHeight() || preview == nullptr) return;

	//OFFSET LARGE SPRITES
	sf::Vector2f offset;
	if(preview->getGlobalBounds().height > TILESIZE){
		offset.y = (preview->getGlobalBounds().height - TILESIZE) / 2;
	}else if(preview->getGlobalBounds().width > TILESIZE){
		offset.x = (preview->getGlobalBounds().width - TILESIZE) / 2;
	}

	preview->setPosition(x * TILESIZE - offset.x, y * TILESIZE - offset.y);

	window.draw(*preview);
}
void EntityPlaceable::townNamePrompt(anax::Entity e){
	int w = 380, h = 300;

	auto townNameWin = std::make_shared<tgui::ChildWindow>();
	townNameWin->setSize(w, h);
	townNameWin->setTitle("Set town name");
	townNameWin->show();
	townNameWin->connect("Closed",[townNameWin]{
		townNameWin->destroy();
	});
	auto msg = std::make_shared<tgui::Label>();
	msg->setText("Enter town name:");
	msg->setSize(w, 28);
	townNameWin->add(msg);
	auto nameField = std::make_shared<tgui::TextBox>();
	nameField->setSize(w, 28);
	townNameWin->add(nameField, "name");
	auto submit = std::make_shared<tgui::Button>();
	submit->setText("Submit");
	submit->connect("Pressed",[e, townNameWin]{
		std::string n = townNameWin->get<tgui::TextBox>("name")->getText();
		(e.getComponent<NameComponent>()).name = n;
		townNameWin->destroy();
	});
}
void EntityPlaceable::setSelection(std::string n, std::string s){
	entityName = n;
	preview = map->getFactory()->getAtlas().createSprite(s);
}
bool EntityPlaceable::checkConditions(){
	return !entityName.empty();
}
void EntityPlaceable::rotateSelection(){

}
void EntityPlaceable::reset(){
	entityName.clear();
	preview.release();
}


