#pragma once

#include "../Composite.hpp"

namespace bt
{

/*
    The Sequence composite ticks each child node in order.
    If a child fails or runs, the sequence returns the same status.
    In the next tick, it will try to run each child in order again.
    If all children succeeds, only then does the sequence succeed.
*/
class Sequence : public Composite
{
public:
	Sequence(Blackboard::Ptr board) : Composite(board){name = "Sequence";}

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
            auto &child = children[index];
            auto status = child->Tick(e, privBoard);
            
            // If the child fails, or keeps running, do the same.
            if (status != Status::Success) {
                return status;
            }

            setIndex(privBoard, ++index);

            // Hit the end of the array, job done!
            if (index == children.size()) {
                return Status::Success;
            }
        }
    }
    
    using Ptr = std::shared_ptr<Sequence>;
};

static Sequence::Ptr MakeSequence(Blackboard::Ptr board)
{
    return std::make_shared<Sequence>(board);
}

}
