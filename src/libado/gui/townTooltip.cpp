/*
 * townTooltip.cpp
 *
 *  Created on: 17 Feb 2017
 *      Author: linbob
 */

#include "townTooltip.hpp"

TownTooltip::TownTooltip(TileMap* map, anax::World* world, WorldView* w_view,
	ScreenManager* manager) : tgui::Widget(), map(map), world(world), w_view(w_view),
	window(manager->getWindow()){

	tooltip = manager->getTheme()->load("Panel");
	tooltip->setSize(194, 182);
	manager->getGui()->add(tooltip);

	setSize(1, 1);
	auto windowWidth = tgui::bindWidth(*manager->getGui());
	auto windowHeight = tgui::bindHeight(*manager->getGui());

	setPosition(windowWidth, windowHeight);
//	hide();
}
TownTooltip::TownTooltip(const TownTooltip& copy) : tgui::Widget(copy),
		map(copy.map), world(copy.world), w_view(copy.w_view), window(copy.window)
		{
	setSize(1, 1);

	setPosition(copy.getPosition().x, copy.getPosition().y);
}
void TownTooltip::mouseMoved(){
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
			entityId = -1;
			tooltip->removeAllWidgets();
			tooltip->hide();
		}
		tileX = x;
		tileY = y;
		delayCount = delayCount.Zero;
	}
}
void TownTooltip::update(sf::Time elapsedTime){
	mouseMoved();

	if(delayCount.asSeconds() < delay) delayCount += elapsedTime;

	if((tileX >= 0 && tileY >= 0 && tileX <= map->getWidth() && tileY <= map->getHeight()) &&
			delayCount.asSeconds() >= delay){
		if(entityId == -1){
			std::vector<EntityRefNode>& list = map->getEntityLayer().getEntitiesAt(tileX, tileY);

			if(!list.empty()){

				for(EntityRefNode& ref : list){
					anax::Entity e = world->getEntity(ref.id);
					if(e.hasComponent<PopulationComponent>()){
						entityId = ref.id;
						NameComponent& n = e.getComponent<NameComponent>();
						PopulationComponent& pop = e.getComponent<PopulationComponent>();
						ResourcesComponent& res = e.getComponent<ResourcesComponent>();

						tgui::Label::Ptr name = std::make_shared<tgui::Label>();
						name->setTextColor(sf::Color(190, 190, 190));
						name->setText(n.name);
						tooltip->add(name, "name");
						name->setPosition(4, 4);

						tgui::Label::Ptr population = std::make_shared<tgui::Label>();
						population->setTextColor(sf::Color(190, 190, 190));
						population->setText("Population: " + std::to_string(pop.value));
						tooltip->add(population, "population");
						population->setPosition(4, 24);

						int count = 2;
						for(auto& r : res.map){
							tgui::Label::Ptr resource = std::make_shared<tgui::Label>();
							resource->setTextSize(14);
							resource->setTextColor(sf::Color(190, 190, 190));

							NameComponent& n = r.second.e.getComponent<NameComponent>();
							StackableComponent& stack = r.second.e.getComponent<StackableComponent>();

							std::stringstream ss;
							ss << std::fixed << std::setprecision(2) << stack.count;

							resource->setText(n.name + ": " + ss.str());
							tooltip->add(resource, n.name);
							if(count == 2){
								resource->setPosition(6, count * 26);
							}else{
								resource->setPosition(6, count * 24);
							}

							++count;
						}

						tooltip->setPosition(mousePos.x + 12, mousePos.y + 12);
						tooltip->show();
						return;
					}
				}
			}
		}else{
			anax::Entity e = world->getEntity(entityId);
			ResourcesComponent& res = e.getComponent<ResourcesComponent>();
			for(auto& r : res.map){
				NameComponent& n = r.second.e.getComponent<NameComponent>();
				tgui::Label::Ptr resource = tooltip->get<tgui::Label>(n.name);

				StackableComponent& stack = r.second.e.getComponent<StackableComponent>();

				std::stringstream ss;
				ss << std::fixed << std::setprecision(2) << stack.count;

				resource->setText(n.name + ": " + ss.str());
			}
		}
	}
}
