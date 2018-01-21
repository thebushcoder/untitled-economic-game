/*
 * input.hpp
 *
 *  Created on: 23 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_INPUT_HPP_
#define LIBADO_INPUT_HPP_

#include <Thor/Input.hpp>

struct ThorInput{
	thor::ActionMap<std::string>& getMap(){
		return map;
	}
	thor::ActionMap<std::string>::CallbackSystem& getActionSys(){
		return a_system;
	}
	void setDelta(sf::Time d){
		delta = d;
	}
	sf::Time getDelta(){
		return delta;
	}
	void setGuiFocused(bool f){
		guiFocused = f;
	}
	bool isGuiFocused(){
		return guiFocused;
	}
private:
	bool guiFocused = false;
	sf::Time delta;
	thor::ActionMap<std::string> map;
	thor::ActionMap<std::string>::CallbackSystem a_system;
};



#endif /* LIBADO_INPUT_HPP_ */
