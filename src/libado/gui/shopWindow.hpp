/*
 * shopWindow.hpp
 *
 *  Created on: 16 Oct 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_SHOPWINDOW_HPP_
#define LIBADO_GUI_SHOPWINDOW_HPP_

#include <iomanip> // setprecision
#include <sstream> // stringstream

#include <TGUI/TGUI.hpp>

#include "../entity/components/moneyComponent.hpp"
#include "../entity/components/valueComponent.hpp"
#include "../entity/components/stackableComponent.hpp"
#include "../entity/lootListManager.hpp"
#include "itemExchangeWin.hpp"

class GameScreen;
class ShopWindow : public ItemExchangeWindow{
public:
	typedef std::shared_ptr<ShopWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const ShopWindow> ConstPtr; ///< Shared constant widget pointer

	ShopWindow() : ItemExchangeWindow(){};
	void init(GameScreen* gs);
	virtual void show(anax::Entity shopEntity);

	////////////////////////////////////
	bool rightClick(float x, float y);
	void loadShopItems(anax::Entity shop);
private:
	int entryHeight = 62;
	anax::World* world = nullptr;
	LootListManager* lootManager;
};

#endif /* LIBADO_GUI_SHOPWINDOW_HPP_ */
