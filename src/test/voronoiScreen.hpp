/*
 * voronoiScreen.hpp
 *
 *  Created on: 10 Dec 2017
 *      Author: linbob
 */

#ifndef TEST_VORONOISCREEN_HPP_
#define TEST_VORONOISCREEN_HPP_

#include <vector>
#include <string>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <Point2.h>
#include <Vector2.h>
#include <VoronoiDiagramGenerator.h>
#include <anax/World.hpp>
#include <anax/Entity.hpp>

#include "../libado/screen.hpp"
#include "../libado/screenManager.hpp"
#include "../libado/map/voronoi/voronoiMap.hpp"
#include "../libado/map/voronoi/terrainGeneration.hpp"
#include "../libado/map/voronoi/noisyEdges.hpp"

#include "../libado/entity/components/kingdomComponent.hpp"
#include "../libado/entity/components/provincesComponent.hpp"
#include "../libado/entity/kingdomType.hpp"
#include "../libado/entity/kingdomUtil.hpp"

class VoronoiScreen : public Screen{
public:
	void init();
	int run();
	virtual void load(std::string path){}
	virtual void save(std::string path = ""){}
	void update(sf::Time& delta);
	void render(sf::Time& delta);
	void resize(sf::Event& e);
	void handleInput(sf::Time& delta);
	void loadResources();

	std::unique_ptr<VoronoiMap::VoronoiMap>& getTerrainVorDia() {
		return terrainVorDia;
	}

private:
	std::unique_ptr<VoronoiMap::VoronoiMap> terrainVorDia = nullptr;

	std::shared_ptr<anax::World> entityWorld;
	std::map<KingdomUtil::KingdomType, anax::Entity> kingdoms;
};

#endif /* TEST_VORONOISCREEN_HPP_ */
