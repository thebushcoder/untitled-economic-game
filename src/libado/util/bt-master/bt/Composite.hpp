#pragma once

#include "Node.hpp"

#define INDEX ":index"

namespace bt
{
class Composite : public Node
{
public:
	Composite(Blackboard::Ptr board) : Node(board) {}
    virtual ~Composite() {}
    
    void AddChild(Node::Ptr child) { children.push_back(child); }
    bool HasNoChildren() const { return children.empty(); }
    int GetIndex(Blackboard::Ptr privBoard) const { return (int)privBoard->GetInt(name + INDEX); }
    void setIndex(Blackboard::Ptr privBoard, int index) const { privBoard->SetInt(name + INDEX, index); }
    
protected:
    Nodes children;
};

}
