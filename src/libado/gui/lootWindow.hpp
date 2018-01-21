/*
 * lootWindow.hpp
 *
 *  Created on: 29 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_LOOTWINDOW_HPP_
#define LIBADO_GUI_LOOTWINDOW_HPP_

#include <TGUI/TGUI.hpp>

//#include "../map/tileMap.hpp"
#include "../map/zone.hpp"
#include "itemInfoWindow.hpp"
#include "itemArea.hpp"

class GameScreen;
class LootWindow : public tgui::ChildWindow{
public:
	typedef std::shared_ptr<LootWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const LootWindow> ConstPtr; ///< Shared constant widget pointer

	LootWindow() : tgui::ChildWindow(){}
	void init(GameScreen* gs);
	void show(float x, float y);
	void updateContents(int x, int y);
	bool rightClick(float x, float y);
private:
	int entryHeight = 62;
	EntityFactory* factory = nullptr;
	Overworld* overWorld = nullptr;
	tgui::Theme::Ptr theme;
	int x, y;
};

#endif /* LIBADO_GUI_LOOTWINDOW_HPP_ */
