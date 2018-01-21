#pragma once

#include "../Decorator.hpp"

namespace bt
{

/*
    The Failer decorator returns failure, regardless of what happens to the child.
*/
class Failer : public Decorator
{
public:
	Failer(Blackboard::Ptr board) : Decorator(board) {}
    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        child->Tick(e, privBoard);
        return Status::Failure;
    }
};

}
