/*
 * ItemTooltip.hpp
 *
 *  Created on: 1 Jul 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_ITEMTOOLTIP_HPP_
#define LIBADO_GUI_ITEMTOOLTIP_HPP_

#include <TGUI/TGUI.hpp>
#include "../entity/components/componentsCollection.hpp"
#include "../entity/components/itemTypes.hpp"

class ItemTooltip : public tgui::Panel{
public:
	typedef std::shared_ptr<ItemTooltip> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ItemTooltip> ConstPtr;

	ItemTooltip(tgui::Theme::Ptr theme);

	void updateContents(anax::Entity& item);

private:
	std::map<std::string, tgui::Panel::Ptr> widgets;
};



#endif /* LIBADO_GUI_ITEMTOOLTIP_HPP_ */
