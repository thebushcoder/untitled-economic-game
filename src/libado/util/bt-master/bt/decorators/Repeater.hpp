#pragma once

#include "../Decorator.hpp"

namespace bt
{

/*
    The Repeater decorator repeats infinitely or to a limit until the child returns success.
*/
class Repeater : public Decorator
{
public:
    Repeater(Blackboard::Ptr sharedBoard, int limit = 0) : Decorator(sharedBoard), limit(limit) {}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        privBoard->SetInt(name + ":counter", 0);
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	int count = privBoard->GetInt(name + ":counter");

        while (1) {
            auto s = child->Tick(e, privBoard);

            if (s == Status::Running) {
                return Status::Running;
            }

            if (s == Status::Failure) {
                return Status::Failure;
            }

            privBoard->SetInt(name + ":counter", ++count);

            if (limit > 0 && count == limit) {
                return Status::Success;
            }

            child->Reset(privBoard);
        }
    }

protected:
    int limit;
};

}
