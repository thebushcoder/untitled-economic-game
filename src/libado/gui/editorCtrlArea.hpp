/*
 * editorCtrlArea.hpp
 *
 *  Created on: 17 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_EDITORCTRLAREA_HPP_
#define LIBADO_GUI_EDITORCTRLAREA_HPP_

#include <TGUI/TGUI.hpp>

#include "spriteButton.hpp"
#include "../screenManager.hpp"
#include "../areaEdit.hpp"
#include "saveFileWindow.hpp"
#include "../../editorScreen.hpp"
#include "../../mainMenu.hpp"

/*
 * 	Editor Control area. Holds tile and entity, Save map, and Exit buttons
 */

namespace EditorCtrlArea{
	void scrollYPanel(tgui::Panel::Ptr panel, tgui::Scrollbar::Ptr scroll);
	void createCtrlArea(EditorScreen* screen);
	void loadTileButtons(EditorScreen* screen);
	void loadEntityButtons(EditorScreen* screen);
}

#endif /* LIBADO_GUI_EDITORCTRLAREA_HPP_ */
