#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../components/mountComponent.hpp"

namespace bt
{

class HasMount : public Decorator
{
public:
	HasMount(Blackboard::Ptr board) :
		Decorator(board){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	MountComponent& m = e.getComponent<MountComponent>();
    	privBoard->SetInt("tgt_entity", m.getMount().getId().index);
    }

    bool conditionCheck(anax::Entity& e, Blackboard::Ptr privBoard){
		if(e.hasComponent<MountComponent>()){
			return true;
		}else{
			return false;
		}
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	return child->Tick(e, privBoard);
    }
};

}
