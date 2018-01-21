/*
 * RegulatorDecotator.hpp
 *
 *  Created on: 14 Aug 2016
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
//#include "../Decorator.hpp"

namespace bt
{

/*
 * a decorator to reset the players 'needsInput' variable
 */
class ResetInput : public Decorator
{
public:
	ResetInput(Blackboard::Ptr Board) : Decorator(Board){}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	return child->Tick(e, privBoard);
    }
    void Terminate(Status s, Blackboard::Ptr privBoard) {
    	privBoard->SetBool("needsInput", true);
    }
};

}
