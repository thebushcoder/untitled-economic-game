/*
 * saveFileWindow.hpp
 *
 *  Created on: 6 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_SAVEFILEWINDOW_HPP_
#define LIBADO_GUI_SAVEFILEWINDOW_HPP_

#include <TGUI/TGUI.hpp>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <memory>

#include <regex>

#include "editorCtrlArea.hpp"

class SaveFileWindow : public tgui::ChildWindow{
public:
	typedef std::shared_ptr<SaveFileWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const SaveFileWindow> ConstPtr; ///< Shared constant widget pointer

	std::string rootPath, instancePath;

	SaveFileWindow() : tgui::ChildWindow(){}
	void init(tgui::Theme::Ptr theme, std::string instPath, bool isLoading);
	void updateList();
};

#endif /* LIBADO_GUI_SAVEFILEWINDOW_HPP_ */
