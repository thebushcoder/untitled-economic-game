/*
 * screen.hpp
 *
 *  Created on: 18 Feb 2016
 *      Author: linbob
 */

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include <string>
#include <map>
#include <memory>

#include <Thor/Input.hpp>
#include <TGUI/TGUI.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "input.hpp"

#define UNINIT 2
#define RUNNING 1
#define QUIT 0

#define MAX_VIEW_W 800
#define MAX_VIEW_H 600

class ScreenManager;

class Screen{
public:
//	Screen(SceneRoot::Ptr r) : root(r){}

	virtual ~Screen(){}

    virtual void init(){}

    virtual int run(){
    	return QUIT;
    }
    // Updates the game
    // \param deltaTime The change in time
    virtual void update(sf::Time& delta){}
    // Renders the game
    virtual void render(){}
    // Handles events
    // \param event The event that will be handled
    virtual void handleInput(sf::Time& delta){}
    // Loads game resources
    virtual void loadResources(){}
    virtual void resize(sf::Event& e){}
    virtual void load(std::string path){}
    virtual void save(std::string path = ""){}

    ScreenManager* getManager(){
    	return manager;
    }
    void setManager(ScreenManager* manager){
    	Screen::manager = manager;
    }
    int getStatus(){
    	return status;
    }
    void setStatus(int s){
    	status = s;
    }
    ThorInput& getInput(){
    	return input;
    }
protected:
    int status = UNINIT;
    ThorInput input;
	ScreenManager* manager = nullptr;
};

#endif /* SCREEN_HPP_ */
