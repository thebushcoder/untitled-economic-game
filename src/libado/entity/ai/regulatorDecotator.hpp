/*
 * RegulatorDecotator.hpp
 *
 *  Created on: 14 Aug 2016
 *      Author: linbob
 */

#pragma once

#include <SFML/System.hpp>
#include "../../util/bt-master/bt.hpp"

namespace bt
{

/*
    The Regulator decorator updates the decorated task at set time intervals.
*/
class Regulator : public Decorator
{
public:
	Regulator(Blackboard::Ptr Board, float l) : Decorator(sharedBoard){
		limit = sf::seconds(l);
	}

    void Initialize(Blackboard& privBoard)
    {
        privBoard.SetFloat(name + ":counter", 0);
    }

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard) override
    {
    	sf::Time count = sf::seconds(privBoard->GetFloat(name + ":counter"));
    	sf::Time delta = sf::seconds(sharedBoard->GetFloat("delta"));

		if((count += delta) >= limit){
			return child->Tick(e, privBoard);
		}else{
			privBoard->SetFloat(name + ":counter", count.asSeconds());
			return Status::Running;
		}
    }

protected:
    sf::Time limit;	//sf::seconds()
};

}
