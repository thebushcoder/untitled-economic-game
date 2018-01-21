#pragma once

#include "../Decorator.hpp"

namespace bt
{

/*
    The Succeeder decorator returns success, regardless of what happens to the child.
*/
class Succeeder : public Decorator
{
public:
	Succeeder(Blackboard::Ptr board) : Decorator(board) {}
    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        child->Tick(e, privBoard);
        return Status::Success;
    }
};

}
