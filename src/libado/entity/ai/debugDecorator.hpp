/*
 * debugDecorator.hpp
 *
 *  Created on: 9 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

class DebugDecorator : public Decorator
{
public:
	DebugDecorator(std::string i, std::string r, Blackboard::Ptr board) :
		Decorator(board), init(i), running(r){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	Logging::printToConsole(init);
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	Logging::printToConsole(running);
		return child->Tick(e, privBoard);
    }
private:
    std::string init, running;
};

}
