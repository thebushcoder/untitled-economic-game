#pragma once

#include "../Decorator.hpp"

namespace bt
{

/*
    The Inverter decorator inverts the child node's status, i.e. failure becomes success and success becomes failure.
    If the child runs, the Inverter returns the status that it is running too.
*/
class Inverter : public Decorator
{
public:
	Inverter(Blackboard::Ptr board) : Decorator(board) {}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        auto s = child->Tick(e, privBoard);

        if (s == Status::Success) {
            return Status::Failure;
        }
        else if (s == Status::Failure) {
            return Status::Success;
        }

        return s;
    }
};

}
