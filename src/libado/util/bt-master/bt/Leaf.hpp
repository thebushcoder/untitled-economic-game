#pragma once

#include "Node.hpp"
#include "Blackboard.hpp"
#include <vector>
#include <memory>

namespace bt
{

class Leaf : public Node
{
public:
    virtual ~Leaf() {}
    Leaf(Blackboard::Ptr blackboard) : Node(blackboard) {}
};

}
