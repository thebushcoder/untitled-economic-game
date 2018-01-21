#pragma once

#include "../Decorator.hpp"

namespace bt
{

/*
    The UntilFail decorator repeats until the child returns fail and then returns success.
*/
class UntilFail : public Decorator
{
public:
	UntilFail(Blackboard::Ptr board) : Decorator(board) {}
    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        while (1) {
            auto status = child->Tick(e, privBoard);

            if (status == Status::Failure) {
                return Status::Success;
            }
        }
    }
};

}
