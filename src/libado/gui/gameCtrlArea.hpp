/*
 * gameCtrlArea.hpp
 *
 *  Created on: 1 Dec 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_GAMECTRLAREA_HPP_
#define LIBADO_GUI_GAMECTRLAREA_HPP_

#include <TGUI/TGUI.hpp>

#include "../screenManager.hpp"

//forward dec to avoid circular dependancy error
class GameScreen;

namespace GameCtrlArea{
	void createCtrlArea(GameScreen* screen);

}

#endif /* LIBADO_GUI_GAMECTRLAREA_HPP_ */
