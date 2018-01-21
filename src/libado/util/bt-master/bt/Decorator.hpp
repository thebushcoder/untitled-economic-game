#pragma once

#include "Node.hpp"

namespace bt
{

class Decorator : public Node
{
public:
	Decorator(Blackboard::Ptr board) : Node(board) {}
    virtual ~Decorator() {}

    void SetChild(Node::Ptr child) { this->child = child; }
    bool HasNoChild() const { return child == nullptr; }
    
protected:
    Node::Ptr child = nullptr;
};

}
