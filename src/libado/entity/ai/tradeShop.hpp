/*
 * pickUpItem.hpp
 *
 *  Created on: 2 Sep 2016
 *      Author: linbob
 */

#pragma once

#include <TGUI/TGUI.hpp>

#include "../../util/bt-master/bt.hpp"
#include "../../gui/shopWindow.hpp"

namespace bt
{
//shows the shop trade window
class TradeShop : public Leaf
{
public:
	TradeShop(tgui::Gui* gui, Blackboard::Ptr board) : Leaf(board), gui(gui){}

	void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
	{
	}

	/*
	 * this needs to show a gui window displaying items at target tile
	 * target item id is unneeded, desired items are selected by played from the above window
	 */
    Status Update(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    	int id = privBoard->GetInt("tgt_entity");
    	anax::Entity i = e.getWorld().getEntity(id);

    	PositionComponent& player = e.getComponent<PositionComponent>();
    	PositionComponent& shop = i.getComponent<PositionComponent>();

//    	printf("\t\t#TRADE WITH SHOP\n");

    	//check player is next to shop
    	if((player.tilePosition.x >= shop.tilePosition.x - 1 && player.tilePosition.x <= shop.tilePosition.x + 1) &&
    			(player.tilePosition.y >= shop.tilePosition.y - 1 && player.tilePosition.y <= shop.tilePosition.y + 1)){
        	ShopWindow::Ptr window = gui->get<ShopWindow>("shop_window", true);
        	window->show(i);

        	privBoard->RemoveInt("tgt_entity");
        	if(e.hasComponent<PlayerComponent>()){
				privBoard->SetBool("needsInput", true);
			}

    		return Status::Success;
    	}

    	privBoard->RemoveInt("tgt_entity");
    	return Status::Failure;
    }
private:
    tgui::Gui* gui;
};

}
