/*
 * widgetLuaWrapper.hpp
 *
 *  Created on: 17 Jul 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_WIDGETLUABIND_HPP_
#define LIBADO_GUI_WIDGETLUABIND_HPP_

#include <TGUI/TGUI.hpp>
#include <sol.hpp>

namespace WidgetLuaBind{
	//C++ > Lua bindings for tgui Widgets
	static void initBindings(sol::state& lua){
		lua.new_usertype<tgui::Widget>("Widget",
				"setName", &tgui::Widget::setName,
				"getName", &tgui::Widget::getName,
				"getX", [](tgui::Widget* w) -> float{
					return w->getPosition().x;
				},
				"getY", [](tgui::Widget* w) -> float{
					return w->getPosition().y;
				},
				"setX", [](tgui::Widget* w, float x){
					w->setPosition(x, w->getPosition().y);
				},
				"setY", [](tgui::Widget* w, float y){
					w->setPosition(w->getPosition().x, y);
				},
				"getWidth", [](tgui::Widget* w) -> float{
					return w->getSize().x;
				},
				"getHeight", [](tgui::Widget* w) -> float{
					return w->getSize().y;
				},
				"setWidth", [](tgui::Widget* w, float x){
					w->setSize(x, w->getSize().y);
				},
				"setHeight", [](tgui::Widget* w, float y){
					w->setSize(w->getSize().x, y);
				},
				"moveX", [](tgui::Widget* w, float x){
					w->move(x, 0);
				},
				"moveY", [](tgui::Widget* w, float y){
					w->move(0, y);
				}
		);
		lua.new_usertype<tgui::Container>("Container",
				"getChild", [](tgui::Container* w, std::string s) -> tgui::Widget*{
					return w->get<tgui::Widget>(s).get();
				},
				sol::base_classes, sol::bases<tgui::Widget>()
		);
	}
};



#endif /* LIBADO_GUI_WIDGETLUABIND_HPP_ */
