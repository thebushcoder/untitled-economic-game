/*
 * editorScreen.hpp
 *
 *  Created on: 13 Mar 2016
 *      Author: linbob
 */

#ifndef EDITORSCREEN_HPP_
#define EDITORSCREEN_HPP_

#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <anax/World.hpp>

#include "libado/screen.hpp"
#include "libado/cursorOutline.hpp"
#include "libado/placement.hpp"
#include "tilePlaceable.hpp"
#include "entityPlaceable.hpp"
#include "libado/areaEdit.hpp"
#include "libado/worldView.hpp"
#include "libado/textureAtlas.hpp"
#include "libado/map/tileMap.hpp"
#include "libado/gui/editorCtrlArea.hpp"
#include "libado/gui/tooltip.hpp"

#define MAP_W 80
#define MAP_H 80

class EditorScreen : public Screen{
public:
	void init();
	int run();
	void update(sf::Time& delta);
	void render(sf::Time& delta);
	void resize(sf::Event& e);
	void handleInput(sf::Time& delta);
	void loadResources();
	virtual void load(std::string path);
	virtual void save(std::string path);

	void createDefaultMap();

	PlacementManager& getPlacement(){
		return *placement;
	}
	TileMap& getTileMap(){
		return *map;
	}
	TextureAtlas& getAtlas(){
		return *atlas;
	}
	WorldView& getWorldView(){
		return *worldView;
	}
	EntityFactory& getEntityFactory(){
		return *entFactory;
	}
	AreaEdit& getAreaEdit(){
		return *areaEdit;
	}
protected:
	std::shared_ptr<WorldView> worldView;
	std::shared_ptr<TextureAtlas> atlas;
	std::shared_ptr<TileMap> map;
	std::shared_ptr<PlacementManager> placement;
	std::shared_ptr<EntityFactory> entFactory;
	std::shared_ptr<CursorOutline> cursorOutline;
	std::shared_ptr<AreaEdit> areaEdit;

	std::shared_ptr<anax::World> entityWorld;
};

#endif /* EDITORSCREEN_HPP_ */
