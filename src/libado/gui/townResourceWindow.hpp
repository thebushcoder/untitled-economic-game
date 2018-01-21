/*
 * townResourceWindow.hpp
 *
 *  Created on: 7 Feb 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_TOWNRESOURCEWINDOW_HPP_
#define LIBADO_GUI_TOWNRESOURCEWINDOW_HPP_

#include <TGUI/TGUI.hpp>
#include <anax/Entity.hpp>
#include <iomanip> // setprecision
#include <sstream>
#include <memory>

#include "spriteWidget.hpp"
#include "../entity/systems/townResourceSys.hpp"
#include "../entity/components/resourcesComponent.hpp"
#include "../entity/components/spriteComponent.hpp"
#include "../entity/components/nameComponent.hpp"
#include "../entity/components/stackableComponent.hpp"

class TownResourceWindow : public tgui::ChildWindow{
protected:
	tgui::Theme::Ptr theme = nullptr;
	TownResourceSys* resSystem;
	int labelH = 38;
	tgui::Color fontColor = {255,255,255};

	void addEntry(ResourcesComponent::Resource& res, tgui::Panel::Ptr townPanel, int row);
	virtual void update(sf::Time elapsedTime) override;
	void updateEntryPositions();
	static void scrollPanel(tgui::ChildWindow* panel, tgui::Scrollbar::Ptr scroll);
public:
	typedef std::shared_ptr<TownResourceWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const TownResourceWindow> ConstPtr; ///< Shared constant widget pointer

	TownResourceWindow() : tgui::ChildWindow(){}
	void init(TownResourceSys* resSys, tgui::Theme::Ptr tq);
	virtual void show();
	//virtual void hide();
};

#endif /* LIBADO_GUI_TOWNRESOURCEWINDOW_HPP_ */
