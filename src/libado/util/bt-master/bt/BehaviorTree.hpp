#pragma once

#include <iostream>
#include <memory>
#include <sol.hpp>
#include "Node.hpp"

namespace bt
{

class BehaviorTree : public Node
{
public:
    BehaviorTree() : Node(std::make_shared<Blackboard>()) {}
    BehaviorTree(const Node::Ptr &rootNode) : BehaviorTree() { root = rootNode; }
    BehaviorTree(const Blackboard::Ptr &shared) : Node(shared) {}
    
    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) { return root->Tick(e, privBoard); }
    
    void SetRoot(bt::Node::Ptr node) {
    	root = node;
    }
    bt::Node::Ptr getRoot(){
    	return root;
    }
private:
    Node::Ptr root = nullptr;
};

}
