/*
 * timeWidget.hpp
 *
 *  Created on: 2 Mar 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_TIMEWIDGET_HPP_
#define LIBADO_GUI_TIMEWIDGET_HPP_

#include <TGUI/TGUI.hpp>

#include "../util/timeSystem.hpp"

namespace TimeWidget{
	tgui::Panel::Ptr createTimeWidget(tgui::Theme::Ptr theme);
}


#endif /* LIBADO_GUI_TIMEWIDGET_HPP_ */
