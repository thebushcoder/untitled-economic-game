/*
 * rideMount.hpp
 *
 *  Created on: 17 Aug 2017
 *      Author: linbob
 */

#pragma once

#include "../../util/bt-master/bt.hpp"
#include "../../map/zone.hpp"

namespace bt
{

class RideMount : public Leaf
{
public:
	RideMount(Blackboard::Ptr board, Overworld* oWorld) : Leaf(board), overWorld(oWorld){}

	void Initialize(anax::Entity& e, Blackboard::Ptr privBoard) override
	{
	}

    Status Update(anax::Entity& e, Blackboard::Ptr privBoard)
    {
    	EquipmentComponent& eq = e.getComponent<EquipmentComponent>();
    	MountComponent& mount = e.getComponent<MountComponent>();

    	EquipmentSlot* es = eq.getSlot(Item::MOUNT);

		//try to equip item
		if(!es->equipItem(mount.getMount())){
			//unable to equip item
			return Status::Failure;
		}

		mount.getMount().deactivate();
		PositionComponent& playerPos = e.getComponent<PositionComponent>();
		PositionComponent& mountPos = mount.getMount().getComponent<PositionComponent>();

		overWorld->getActiveZone()->zone->getEntityLayer().removeEntity(playerPos.tilePosition.x,
				playerPos.tilePosition.y, e.getId().index);

		//reposition entity
		playerPos.setPosition(mountPos.tilePosition.x, mountPos.tilePosition.y,
				mountPos.tilePosition.x * TILESIZE, mountPos.tilePosition.y * TILESIZE);

		//add entity to tile
		overWorld->getActiveZone()->zone->getEntityLayer().setEntity(
				mountPos.tilePosition.x, mountPos.tilePosition.y, e);

		//remove mount from zone map
		overWorld->getActiveZone()->zone->getEntityLayer().removeEntity(mountPos.tilePosition.x,
				mountPos.tilePosition.y, mount.getMount().getId().index);

		privBoard->RemoveInt("tgt_entity");

		return Status::Success;
    }
private:
    Overworld* overWorld;
};

}
