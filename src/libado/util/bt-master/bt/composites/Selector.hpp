#pragma once

#include "../Composite.hpp"

namespace bt
{

/*
    The Selector composite ticks each child node in order.
    If a child succeeds or runs, the sequence returns the same status.
    In the next tick, it will try to run each child in order again.
    If all children fails, only then does the selector fail.
*/
class Selector : public Composite
{
public:
	Selector(Blackboard::Ptr board) : Composite(board){}

    void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	setIndex(privBoard, 0);
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
        if (HasNoChildren()) {
            return Status::Success;
        }

        int index = GetIndex(privBoard);

        // Keep going until a child behavior says it's running.
        while (1) {
            auto &child = children.at(index);
            auto status = child->Tick(e, privBoard);

            // If the child succeeds, or keeps running, do the same.
            if (status != Status::Failure) {
                return status;
            }

            // Hit the end of the array, it didn't end well...
            if (index + 1 >= children.size()) {
                return Status::Failure;
            }else{
            	setIndex(privBoard, ++index);
            }
        }
    }
    
    using Ptr = std::shared_ptr<Selector>;
};

static Selector::Ptr MakeSelector(Blackboard::Ptr board)
{
    return std::make_shared<Selector>(board);
}

}
