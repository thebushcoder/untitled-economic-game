/*
 * AiSystem.hpp
 *
 *  Created on: 14 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_AISYSTEM_HPP_
#define LIBADO_ENTITY_SYSTEMS_AISYSTEM_HPP_

#include <anax/System.hpp>
#include <queue>

#include "../../util/bt-master/bt.hpp"
#include "../../map/tileMap.hpp"
#include "../ai/walkToTgt.hpp"
#include "../ai/idleTask.hpp"
#include "../ai/pickUpItem.hpp"
#include "../ai/pathToTgt.hpp"
#include "../ai/tradeShop.hpp"
#include "../ai/randomDecorator.hpp"
#include "../ai/tgtRandomTile.hpp"
#include "../ai/attackTgt.hpp"
#include "../ai/takeDmgChk.hpp"
#include "../ai/taskCheckDecorator.hpp"
#include "../ai/resetPlayerIn.hpp"
#include "../ai/tgtOwner.hpp"
#include "../components/aiComponent.hpp"
#include "../components/energyComponent.hpp"

#include "../../util/fileLogger.hpp"
#include "townResourceSys.hpp"
#include "../../util/timeSystem.hpp"
#include "../ai/chkTgtDistance.hpp"
#include "../ai/hasOwner.hpp"

/*	Turn based behaviour tree system
 * Ref
 * 	https://github.com/munificent/hauberk/blob/master/lib/src/engine/game.dart
 * 	http://journal.stuffwithstuff.com/2014/07/15/a-turn-based-game-loop/
 */
struct AiSystem : anax::System<anax::Requires<AiComponent, EnergyComponent>>{
private:
	Overworld* overWorld = nullptr;
	EntityFactory* factory = nullptr;
	std::map<std::string, std::shared_ptr<bt::BehaviorTree>> allTrees;

public:
	static constexpr int productionTick = 32;
	static constexpr int stdEnergyRec = 100;
	static constexpr int stdTurnCost = 100;
	static float gameSpeed;
	int worldEnergy = 0, worldTurnCount = 0;

	AiSystem(){}
	AiSystem(Overworld* overWorld, EntityFactory* factory, tgui::Gui* gui);

	void processNpcTurn(anax::Entity e);

	int debug_playerCount = 0;

	void process(sf::Time delta);
	void processWorldTick();
	static void setSpeed(float s);
	void addTree(std::shared_ptr<bt::BehaviorTree> tree, std::string name){
		allTrees[name] = tree;
	}
	bt::BehaviorTree* getTree(std::string name){
		return allTrees[name].get();
	}
	std::map<std::string, std::shared_ptr<bt::BehaviorTree>>& getAllTrees(){
		return allTrees;
	}
};

#endif /* LIBADO_ENTITY_SYSTEMS_AISYSTEM_HPP_ */
