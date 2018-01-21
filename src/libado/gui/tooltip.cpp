/*
 * tooltip.cpp
 *
 *  Created on: 24 Sep 2016
 *      Author: linbob
 */
#include "tooltip.hpp"

TooltipAnchor::TooltipAnchor(TileMap* map, anax::World* world, WorldView* w_view,
		ScreenManager* manager) : tgui::Widget(),
		map(map), world(world), w_view(w_view), window(manager->getWindow()){
	tooltip = manager->getTheme()->load("CustomTooltip");
	tooltip->setTextSize(14);
	manager->getGui()->add(tooltip);

	setSize(1, 1);
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	setPosition(windowWidth, windowHeight);
}

TooltipAnchor::TooltipAnchor(const TooltipAnchor& copy) : tgui::Widget(copy),
		map(copy.map), world(copy.world), w_view(copy.w_view), window(copy.window),
		tooltip(copy.tooltip){
	setSize(1, 1);

	setPosition(copy.getPosition().x, copy.getPosition().y);
}
void TooltipAnchor::mouseMoved(){
	mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2f worldVec = window->mapPixelToCoords(mousePos, *(w_view->view));

	//tile position
	int x = std::round(worldVec.x / TILESIZE);
	int y = std::round(worldVec.y / TILESIZE);

	//check map bounds
	if(x < 0 || y < 0 || x >= map->getWidth() || y >= map->getHeight()) return;

	if(x != tileX || y != tileY){
		//new tile position
		if(isVisible()){
			tooltip->setText("");
			tooltip->hide();
		}
		tileX = x;
		tileY = y;
		delayCount = delayCount.Zero;
	}
}
void TooltipAnchor::update(sf::Time elapsedTime){
	if(!isVisible()){
		delayCount += elapsedTime;
		if((tileX >= 0 && tileY >= 0 && tileX <= map->getWidth() && tileY <= map->getHeight()) &&
				delayCount.asSeconds() >= delay){
			std::string text = "";
			std::vector<EntityRefNode>& list = map->getEntityLayer().getEntitiesAt(tileX, tileY);

			if(!list.empty()){

				for(EntityRefNode& ref : list){
					anax::Entity e = world->getEntity(ref.id);
					text += e.getComponent<NameComponent>().name;
					if(e.hasComponent<StackableComponent>()){
						StackableComponent& s = e.getComponent<StackableComponent>();
						text += "\t x" + std::to_string(s.count);
					}
					text += "\n";
				}

				tooltip->setSize(180, list.size() * 26);
				tooltip->setText(text);
				tooltip->setPosition(mousePos.x + 12, mousePos.y + 12);
				tooltip->show();
			}
		}
	}else if(isVisible()){
		mouseMoved();
	}
}

