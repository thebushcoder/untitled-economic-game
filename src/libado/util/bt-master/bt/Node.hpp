#ifndef NODE_HPP_
#define NODE_HPP_

#pragma once

#include <memory>
#include <vector>

#include <anax/Entity.hpp>

#include "Blackboard.hpp"

#define STATUS ":status"

namespace bt
{

class Node
{
protected:
    Blackboard::Ptr sharedBoard;
    std::string name;
    int id;
    static int nextId;
public:
    enum class Status
    {
        Invalid,
        Success,
        Failure,
        Running,
    };

    Node(Blackboard::Ptr board) : sharedBoard(board), name("Node"), id(nextId++){}
    Node(const Node& n) : sharedBoard(n.sharedBoard), name(n.name), id(n.id){}
    virtual ~Node() {}

    virtual Status Update(anax::Entity& e, Blackboard::Ptr privBoard) = 0;
    virtual void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) {}
    virtual void Terminate(Status s, Blackboard::Ptr privBoard) {}

    Status Tick(anax::Entity& e, Blackboard::Ptr privBoard)
    {
        if ((Status)privBoard->GetInt(getNameStatus()) != Status::Running) {
        	if(!conditionCheck(e, privBoard)) return Status::Failure;

            Initialize(e, privBoard);
        }

        privBoard->SetInt(getNameStatus(), (int)Update(e, privBoard));

        if ((Status)privBoard->GetInt(getNameStatus()) != Status::Running) {
            Terminate((Status)privBoard->GetInt(getNameStatus()), privBoard);
        }

        return (Status)privBoard->GetInt(getNameStatus());
    }
    virtual bool conditionCheck(anax::Entity& e, Blackboard::Ptr privBoard){
    	return true;
    }

    bool IsSuccess(Blackboard::Ptr privBoard) const {
    	return (Status)privBoard->GetInt(name + std::to_string(id) + STATUS) == Status::Success;
    }
    bool IsFailure(Blackboard::Ptr privBoard) const {
    	return (Status)privBoard->GetInt(name + std::to_string(id) + STATUS) == Status::Failure;
    }
    bool IsRunning(Blackboard::Ptr privBoard) const {
    	return (Status)privBoard->GetInt(name + std::to_string(id) + STATUS) == Status::Running;
    }
    bool IsTerminated(Blackboard::Ptr privBoard) const {
    	return IsSuccess(privBoard) || IsFailure(privBoard);
    }
    void Reset(Blackboard::Ptr privBoard) {
    	privBoard->SetInt(name + std::to_string(id) + STATUS, (int)Status::Invalid);
    }

    void setName(std::string name){this->name = name;}
    std::string getName(){return name;}

    std::string getNameStatus(){
    	return name + std::to_string(id) + STATUS;
    }

    Blackboard::Ptr GetSharedBlackboard() const { return sharedBoard; }
	void SetSharedBlackboard(const Blackboard::Ptr &shared) { sharedBoard = shared; }

    using Ptr = std::shared_ptr<Node>;

    int getId(){
    	return id;
    }
};

using Nodes = std::vector<Node::Ptr>;

}

#endif
