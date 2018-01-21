/*
 * itemExchangeWin.hpp
 *
 *  Created on: 21 Sep 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_ITEMEXCHANGEWIN_HPP_
#define LIBADO_GUI_ITEMEXCHANGEWIN_HPP_

#include <TGUI/TGUI.hpp>
#include "itemArea.hpp"

class GameScreen;
class ItemExchangeWindow : public tgui::ChildWindow{
public:
	typedef std::shared_ptr<ItemExchangeWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ItemExchangeWindow> ConstPtr; ///< Shared constant widget pointer

	ItemExchangeWindow() : tgui::ChildWindow(){}
	void init(GameScreen* gs);
	virtual void show(anax::Entity e);
	void setLeftLabel(std::string s){
		get<tgui::Label>("left_label")->setText(s);
	}
	void setRightLabel(std::string s){
		get<tgui::Label>("right_label")->setText(s);
	}
protected:
	EntityFactory* factory = nullptr;
	tgui::Theme::Ptr theme;
private:
	int entryHeight = 62;
};

#endif /* LIBADO_GUI_ITEMEXCHANGEWIN_HPP_ */
