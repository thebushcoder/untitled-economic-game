/*
 * gameScreen.hpp
 *
 *  Created on: 20 Feb 2016
 *      Author: linbob
 */

#ifndef GAMESCREEN_HPP_
#define GAMESCREEN_HPP_

#include <vector>
#include <string>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <anax/World.hpp>
#include <anax/Entity.hpp>
#include <TGUI/TGUI.hpp>

#include <lua.hpp>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>

#include "VoronoiDiagramGenerator.h"

#include "mainMenu.hpp"
#include "libado/screen.hpp"
#include "libado/worldView.hpp"
#include "libado/textureAtlas.hpp"
#include "libado/gui/playerContext.hpp"
#include "libado/gui/lootWindow.hpp"
#include "libado/gui/shopWindow.hpp"
#include "libado/gui/inventoryWindow.hpp"
#include "libado/gui/itemInfoWindow.hpp"
#include "libado/map/zone.hpp"
#include "libado/entity/entityFactory.hpp"
#include "libado/entity/lootListManager.hpp"
#include "libado/entity/systems/AiSystem.hpp"
#include "libado/entity/components/spriteComponent.hpp"
#include "libado/gui/gameCtrlArea.hpp"
#include "libado/gui/townResourceWindow.hpp"
#include "libado/gui/townTooltip.hpp"
#include "libado/gui/timeWidget.hpp"
#include "libado/gui/widgetLuaBind.hpp"
#include "libado/gui/mountInvenWin.hpp"

#include "libado/util/timeSystem.hpp"

class GameScreen : public Screen{
public:
	void init();
	int run();
	virtual void load(std::string path);
	virtual void save(std::string path = "");
	void update(sf::Time& delta);
	void render(sf::Time& delta);
	void resize(sf::Event& e);
	void handleInput(sf::Time& delta);
	void loadResources();

	EntityFactory& getEntFactory(){
		return *entFactory;
	}
	Overworld* getOverworld(){
		return overworld.get();
	}
	AiSystem* getAiSys(){
		return aiSys.get();
	}
	anax::World* getWorld(){
		return entityWorld.get();
	}
	LootListManager* getLootManager(){
		return lootManager.get();
	}
protected:
	std::shared_ptr<TextureAtlas> atlas;
	std::shared_ptr<EntityFactory> entFactory;
	std::shared_ptr<LootListManager> lootManager;

	std::shared_ptr<anax::World> entityWorld;

	std::shared_ptr<AiSystem> aiSys;

	std::unique_ptr<Overworld> overworld;

	sol::state lua;
};

#endif /* GAMESCREEN_HPP_ */
