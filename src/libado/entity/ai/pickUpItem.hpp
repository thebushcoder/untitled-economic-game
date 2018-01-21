/*
 * pickUpItem.hpp
 *
 *  Created on: 2 Sep 2016
 *      Author: linbob
 */

#pragma once

#include <TGUI/TGUI.hpp>

#include "../../util/bt-master/bt.hpp"
#include "../../gui/lootWindow.hpp"

namespace bt
{

class PickUpItem : public Leaf
{
public:
	PickUpItem(tgui::Gui* gui, Blackboard::Ptr board) : Leaf(board), gui(gui){}

	void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
	{
	}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    	int id = privBoard->GetInt("tgt_entity");
    	anax::Entity i = e.getWorld().getEntity(id);
    	PositionComponent& p = i.getComponent<PositionComponent>();

//    	printf("\t\t#PICK UP ITEM\n");

    	LootWindow::Ptr window = gui->get<LootWindow>("loot_window", true);
    	window->show(p.tilePosition.x, p.tilePosition.y);

    	privBoard->RemoveInt("tgt_entity");

		return Status::Success;
    }
private:
    tgui::Gui* gui;
};

}
