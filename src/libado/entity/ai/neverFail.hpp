/*
 * neverFail.hpp
 *
 *  Created on: 17 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt/Decorator.hpp"

namespace bt
{

class NeverFail : public Decorator
{
public:
	NeverFail(Blackboard::Ptr board) : Decorator(board) {}
    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        Status s = child->Tick(e, privBoard);
        if(s == Status::Failure) return Status::Success;

        return s;
    }
};

}
