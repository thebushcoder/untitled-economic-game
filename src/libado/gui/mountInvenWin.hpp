/*
 * mountInvenWin.hpp
 *
 *  Created on: 23 Sep 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_MOUNTINVENWIN_HPP_
#define LIBADO_GUI_MOUNTINVENWIN_HPP_

#include <iomanip> // setprecision
#include <sstream> // stringstream

#include <TGUI/TGUI.hpp>

#include "../entity/components/moneyComponent.hpp"
#include "../entity/components/valueComponent.hpp"
#include "../entity/components/stackableComponent.hpp"
#include "../entity/lootListManager.hpp"
#include "itemExchangeWin.hpp"

class GameScreen;
class MountInvenWindow : public ItemExchangeWindow{
public:
	typedef std::shared_ptr<MountInvenWindow> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const MountInvenWindow> ConstPtr; ///< Shared constant widget pointer

	MountInvenWindow() : ItemExchangeWindow(){};
	void init(GameScreen* gs);
	virtual void show(anax::Entity mount);

	////////////////////////////////////
	bool rightClick(float x, float y);
	void loadShopItems(anax::Entity shop);
private:
	int entryHeight = 62;
	anax::World* world = nullptr;
};

#endif /* LIBADO_GUI_MOUNTINVENWIN_HPP_ */
