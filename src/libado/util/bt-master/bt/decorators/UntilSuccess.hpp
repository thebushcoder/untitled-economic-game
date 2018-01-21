#pragma once

#include "../Decorator.hpp"

namespace bt
{

/*
    The UntilSuccess decorator repeats until the child returns success and then returns success.
*/
class UntilSuccess : public Decorator
{
public:
	UntilSuccess(Blackboard::Ptr board) : Decorator(board) {}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        while (1) {
            auto status = child->Tick(e, privBoard);

            if (status == Status::Success) {
                return Status::Success;
            }
        }
    }
};

}
