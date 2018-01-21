/*
 * itemInfoWindow.hpp
 *
 *  Created on: 11 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_ITEMINFOWINDOW_HPP_
#define LIBADO_GUI_ITEMINFOWINDOW_HPP_

#include <TGUI/TGUI.hpp>
#include <anax/Entity.hpp>

#include "spriteWidget.hpp"
#include "../entity/components/itemTypes.hpp"
#include "../entity/components/spriteComponent.hpp"
#include "../entity/components/weightComponent.hpp"
#include "../entity/components/nameComponent.hpp"
#include "../entity/components/descriptionComponent.hpp"
#include "../entity/components/valueComponent.hpp"
#include "../entity/components/itemComponent.hpp"

class ItemInfoWindow : public tgui::ChildWindow{
protected:
	tgui::Theme::Ptr theme = nullptr;
public:
	typedef std::shared_ptr<ItemInfoWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ItemInfoWindow> ConstPtr; ///< Shared constant widget pointer

	ItemInfoWindow() : tgui::ChildWindow(){}
	void init(tgui::Theme::Ptr theme);
	virtual void show(anax::Entity item);
	virtual void hide();
};

#endif /* LIBADO_GUI_ITEMINFOWINDOW_HPP_ */
