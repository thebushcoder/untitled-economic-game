/*
 * inventoryWindow.hpp
 *
 *  Created on: 7 Nov 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_INVENTORYWINDOW_HPP_
#define LIBADO_GUI_INVENTORYWINDOW_HPP_

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <TGUI/TGUI.hpp>

#include "spriteWidget.hpp"
#include "../entity/entityFactory.hpp"
#include "../map/zone.hpp"
#include "itemInfoWindow.hpp"
#include "slotWidget.hpp"
#include "itemTooltip.hpp"
#include "itemArea.hpp"
//#include "../entity/components/inventoryComponent.hpp"

class GameScreen;
class InventoryWindow : public tgui::ChildWindow{
public:
	typedef std::shared_ptr<InventoryWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const InventoryWindow> ConstPtr; ///< Shared constant widget pointer

	////////////////////////////////////

	InventoryWindow() : tgui::ChildWindow(){}
	void init(GameScreen* gs);
	void equipItem(ItemEntry::Ptr i);
	void dropItem(ItemEntry::Ptr i);

	virtual void show();
	bool rightClick(float x, float y);
	void updateEntries();
	void attachTooltip(tgui::Widget::Ptr w){
		w->setToolTip(itemTooltip);
	}
	ItemTooltip::Ptr getItemToolTip(){
		return itemTooltip;
	}

	int entryHeight = 62;
protected:
	tgui::Theme::Ptr theme = nullptr;
private:
	static const int MAX_TRINKETS = 4;

	GameScreen* gameScreen = nullptr;
	ItemTooltip::Ptr itemTooltip = nullptr;
	std::vector<SlotWidget::Ptr> equipmentSlots;
};

#endif /* LIBADO_GUI_INVENTORYWINDOW_HPP_ */
