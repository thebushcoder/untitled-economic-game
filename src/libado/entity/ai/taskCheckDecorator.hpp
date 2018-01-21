/*
 * taskCheckDecorator.hpp
 *
 *  Created on: 24 Mar 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_AI_TASKCHECKDECORATOR_HPP_
#define LIBADO_ENTITY_AI_TASKCHECKDECORATOR_HPP_

#pragma once

#include "../../util/bt-master/bt.hpp"

namespace bt
{

/*
    Check if new task has been issued. Halt previous task and start new task
*/
class TaskCheckDecorator : public Decorator
{
public:
	TaskCheckDecorator(std::string tn, Blackboard::Ptr Board) :
		Decorator(Board), taskName(tn){}

//    void Initialize(Blackboard& privBoard) override
//    {
//    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
		if(privBoard->GetString("current_task").compare(taskName) == 0){
			return child->Tick(e, privBoard);
		}
		return Status::Failure;
    }
private:
    std::string taskName;
};

}

#endif /* LIBADO_ENTITY_AI_TASKCHECKDECORATOR_HPP_ */
