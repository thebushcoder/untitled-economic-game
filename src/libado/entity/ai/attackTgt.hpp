/*
 * attackTgt.hpp
 *
 *  Created on: 19 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_AI_ATTACKTGT_HPP_
#define LIBADO_ENTITY_AI_ATTACKTGT_HPP_

namespace bt
{

class AttackTgt : public Leaf
{
public:
	AttackTgt(Overworld* overWorld, Blackboard::Ptr board) :
		Leaf(board), overWorld(overWorld){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	int tgtId = privBoard->GetInt("tgt_entity");
    	anax::Entity tgt = e.getWorld().getEntity(tgtId);

    	AttackComponent& atk = e.getComponent<AttackComponent>();
    	EnergyComponent& energy = e.getComponent<EnergyComponent>();
    	HealthComponent& hp = tgt.getComponent<HealthComponent>();

    	hp.currentValue -= atk.baseValue;

    	if(hp.currentValue <= 0){
    		PositionComponent& pos = tgt.getComponent<PositionComponent>();
    		overWorld->getActiveZone()->zone->getEntityLayer().removeEntity(pos.tilePosition.x, pos.tilePosition.y,
    				tgt.getId().index);
    		tgt.kill();
    	}

    	//CONSUME ENTITY ENERGY
		energy.count -= 100;

		privBoard->RemoveInt("tgt_entity");
		if(e.hasComponent<PlayerComponent>()){
			privBoard->SetBool("needsInput", true);
		}

//    	printf("\n\nATTACK\n");
		return Status::Success;
    }
protected:
    Overworld* overWorld;
};

}

#endif /* LIBADO_ENTITY_AI_ATTACKTGT_HPP_ */
