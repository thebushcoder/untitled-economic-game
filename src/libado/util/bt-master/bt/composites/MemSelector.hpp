#pragma once

#include "../Composite.hpp"

namespace bt
{

/*
    The Selector composite ticks each child node in order, and remembers what child it prevously tried to tick.
    If a child succeeds or runs, the sequence returns the same status.
    In the next tick, it will try to run each child in order again.
    If all children fails, only then does the selector fail.
*/
class MemSelector : public Composite
{
public:
	MemSelector(Blackboard::Ptr board) : Composite(board){}

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

            setIndex(privBoard, ++index);

            // Hit the end of the array, it didn't end well...
            if (index == children.size()) {
            	setIndex(privBoard, 0);
                return Status::Failure;
            }
        }
    }
    
    using Ptr = std::shared_ptr<MemSelector>;
};

static MemSelector::Ptr MakeMemSelector(Blackboard::Ptr board)
{
    return std::make_shared<MemSelector>(board);
}

}
