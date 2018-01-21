/*
 * eventListener.hpp
 *
 *  Created on: 16 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_UTIL_EVENTLISTENER_HPP_
#define LIBADO_UTIL_EVENTLISTENER_HPP_

using listenerFunc = std::function<bool(sf::Event* e, sf::Time& delta)>;

struct EventListener{
	static constexpr int KEYBOARD = 0;
	static constexpr int MOUSE = 1;

	std::string name;
	listenerFunc inputFunction;

	sf::Event::EventType event;
	int type;
	sf::Keyboard::Key keyCode;
	sf::Mouse::Button mouseButton;

	EventListener(std::string n, int t) :
		name(n), type(t){}

	EventListener(std::string n, int t, listenerFunc f) :
		name(n), inputFunction(f), type(t){}
	virtual ~EventListener(){}

	virtual bool run(sf::Time& delta){
		return inputFunction(nullptr, delta);
	}
	virtual bool run(sf::Event& e, sf::Time& delta){
		return inputFunction(&e, delta);
	}
	virtual bool operator < (const EventListener& l) const{
		return (name < l.name);
	}
};

struct GuiListener : EventListener{
	int zHeight = 0;

	GuiListener(std::string n, int zHeight, sf::Event::EventType e, listenerFunc f) :
		EventListener(n, 0, f), zHeight(zHeight){
		event = e;
	}

	~GuiListener(){}

	virtual bool run(sf::Event& e, sf::Time& delta){
		if(e.type != event) return false;

		return inputFunction(&e, delta);
	}
	virtual bool operator < (const GuiListener& l) const{
		return (zHeight < l.zHeight);
	}
};

#endif /* LIBADO_UTIL_EVENTLISTENER_HPP_ */
