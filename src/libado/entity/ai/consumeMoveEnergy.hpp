/*
 * consumeMoveEnergy.hpp
 *
 *  Created on: 2 Sep 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../components/equipmentComponent.hpp"

namespace bt
{

class ConsumeMoveEnergy : public Decorator
{
public:
	ConsumeMoveEnergy(Overworld* oWorld, Blackboard::Ptr board) :
		Decorator(board), overWorld(oWorld){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
		EnergyComponent& energy = e.getComponent<EnergyComponent>();

    	Status s = child->Tick(e, privBoard);

    	int cost = privBoard->GetInt("tile_cost");

    	if(e.hasComponent<EquipmentComponent>()){
    		//entity has a mount
    		EquipmentComponent& eq = e.getComponent<EquipmentComponent>();
			if(e.hasComponent<MountComponent>() && eq.getSlot(Item::MOUNT)->hasItem()){
				//entity is riding mount - use mount's move speed
				MountComponent& mount = e.getComponent<MountComponent>();
				MoveSpeedComponent& speed = mount.getMount().getComponent<MoveSpeedComponent>();

				float diff = 1.0 - speed.baseValue;
				cost += std::round(cost * diff);
			}
    	}else{
    		//unmounted entity - use base move speed
    		MoveSpeedComponent& speed = e.getComponent<MoveSpeedComponent>();

    		float diff = 1.0 - speed.baseValue;
			cost += std::round(cost * diff);
    	}

    	if(overWorld->isChildActive()){
			//CONSUME ENTITY ENERGY
			energy.count -= cost;
		}else{
			//CONSUME ENTITY ENERGY - MOVEMENT IN OVERWORLD COSTS MORE PER TILE
			//2 production ticks per overworld tile; 3.5 tiles = 1 day(human walking speed{
			//mounted on horse/wagon will move faster, therefore more tiles per day})
			energy.count -= cost * overWorldMove;
		}

    	return s;
    }
private:
    const static int overWorldMove = 32 * 2;
    Overworld* overWorld;
};

}
