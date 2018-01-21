/*
 * AiSystem.cpp
 *
 *  Created on: 15 Nov 2016
 *      Author: linbob
 */

#include "AiSystem.hpp"

float AiSystem::gameSpeed = 1;

AiSystem::AiSystem(Overworld* overWorld, EntityFactory* factory, tgui::Gui* gui) :
	overWorld(overWorld), factory(factory){
}

void AiSystem::processNpcTurn(anax::Entity e){
	NameComponent& name = e.getComponent<NameComponent>();
	AiComponent& ai = e.getComponent<AiComponent>();
	EnergyComponent& energy = e.getComponent<EnergyComponent>();

	if(energy.count < AiSystem::stdTurnCost){
		std::string s = "AiSystem > \t" + name.name + " energy gain";
		FileLogger::getInstance()->logString(s, "AiSystem.log");
		energy.count += energy.energyRecovery;
	}

	if(energy.count >= AiSystem::stdTurnCost){
		std::string s = "AiSystem > \tProcess " + name.name + " BT : +turn";
		FileLogger::getInstance()->logString(s, "AiSystem.log");
		allTrees[ai.treeName]->Update(e, ai.board);
	}
}

void AiSystem::process(sf::Time delta){
	//IMPORTANT - makes delta value available to all nodes in all trees
	for(auto tree: allTrees){
		tree.second->GetSharedBlackboard()->SetFloat("delta", delta.asSeconds());
	}

	FileLogger::getInstance()->logString("\n======================================================\n", "AiSystem.log");
	FileLogger::getInstance()->logString("AiSystem > processing", "AiSystem.log");

	anax::Entity player = factory->getPlayer();
	AiComponent& ai = player.getComponent<AiComponent>();
	EnergyComponent& energy = player.getComponent<EnergyComponent>();

	//player energy gain
	if(energy.count < AiSystem::stdTurnCost){
		FileLogger::getInstance()->logString("AiSystem > \tPlayer energy gain", "AiSystem.log");
		energy.count += energy.energyRecovery;
	}

	//halt all ai processing(NPC) until player does something/consumes a turn
	if(energy.count >= stdTurnCost && ai.board->GetBool("needsInput")){
		FileLogger::getInstance()->logString("AiSystem > \tPAUSED: Waiting for player to make a move!", "AiSystem.log");
		return;
	}

	//track game turn count
	processWorldTick();

	//update player if enough energy is available, else update npc AI
	if(energy.count >= AiSystem::stdTurnCost){

		if(ai.board->GetBool("needsInput")) return;

		//track player turn count
		++debug_playerCount;
		FileLogger::getInstance()->logString("AiSystem > \tProcess player BT : +turn", "AiSystem.log");
		//process player turn
		allTrees[ai.treeName]->Update(player, ai.board);
	}

	auto entities = getEntities();
    for(auto& e : entities){
		if(e.hasComponent<PlayerComponent>()) continue;

		processNpcTurn(e);
	}
}
void AiSystem::processWorldTick(){
	FileLogger::getInstance()->logString("AiSystem > \tprocessWorldTick", "AiSystem.log");
	if((worldEnergy += stdEnergyRec * gameSpeed)
			>= stdTurnCost){
		FileLogger::getInstance()->logString("AiSystem > \tprocessWorldTick > +1 turn", "AiSystem.log");
		//reset energy counter
		worldEnergy = 0;

		++worldTurnCount;

		FileLogger::getInstance()->logString("AI SYSTEM : new world turn\n");
		FileLogger::getInstance()->logString("AI SYSTEM : Total game turns : " +
				std::to_string(worldTurnCount) + "\n");
		if(worldTurnCount % productionTick == 0){
			FileLogger::getInstance()->logString("\t production tick # :" +
								std::to_string(worldTurnCount / productionTick) + "\n");
			TimeSystem::getInstance()->update();
//			TownResourceSys::getInstance()->process();
		}
	}
}
void AiSystem::setSpeed(float s){
	gameSpeed = s;
}
