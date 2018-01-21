/*
 * playerContext.cpp
 *
 *  Created on: 25 Aug 2016
 *      Author: linbob
 */

#include "playerContext.hpp"
#include "../../gameScreen.hpp"

PlayerContext::PlayerContext(GameScreen* gs) : tgui::Panel(), gameScreen(gs),
	theme(gs->getManager()->getTheme()){

	setSize(180, buttonHeight * 5);
	setBackgroundColor(sf::Color(200, 200, 200));

	//////////////
	createButton("Walk To", "walk_to", [&]{
		AiComponent& ai = gameScreen->getEntFactory().getPlayer().getComponent<AiComponent>();

		ai.board->SetVector("tgt_tile", sf::Vector2i(clickX, clickY));
		ai.board->SetBool("needsInput", false);

		this->hide();
		return;
	});

	//////////////
	createButton("Pick Up", "pick_up", [&]{
		this->hide();
		AiComponent& ai = gameScreen->getEntFactory().getPlayer().getComponent<AiComponent>();

		for(EntityRefNode& ref : gameScreen->getOverworld()->getActiveZone()->zone->getEntityLayer().getEntitiesAt(clickX, clickY)){
			anax::Entity e = gameScreen->getWorld()->getEntity(ref.id);
			std::string refName = e.getComponent<NameComponent>().name;
			if(refName.compare("Loot drop") == 0){
				ai.board->SetString("current_task", "tgt_item");
				ai.board->SetVector("tgt_tile", sf::Vector2i(clickX, clickY));
				ai.board->SetInt("tgt_entity", e.getId().index);
				ai.board->SetBool("needsInput", false);
				return;
			}
		}
	});

	//////////////
	createButton("Ride", "ride", [&]{
		this->hide();

		anax::Entity e = gameScreen->getEntFactory().getPlayer();
		AiComponent& ai = e.getComponent<AiComponent>();
		MountComponent& mount = e.getComponent<MountComponent>();

		ai.board->SetString("current_task", "ride_mount");
		ai.board->SetVector("tgt_tile", sf::Vector2i(clickX, clickY));
		ai.board->SetInt("tgt_entity", mount.getMount().getId().index);
		ai.board->SetBool("needsInput", false);
		return;
	});

	//////////////
	createButton("Claim", "claim", [&]{
		for(EntityRefNode& ref : gameScreen->getOverworld()->getActiveZone()->zone->
				getEntityLayer().getEntitiesAt(clickX, clickY)){

			anax::Entity e = gameScreen->getWorld()->getEntity(ref.id);
			if(e.hasComponent<ItemComponent>()){
				ItemComponent& i = e.getComponent<ItemComponent>();
				if(i.hasType(Item::MOUNT)){
					anax::Entity player = gameScreen->getEntFactory().getPlayer();

					e.addComponent<OwnerComponent>(player);
					player.addComponent<MountComponent>(&gameScreen->getEntFactory(), e);
				}
			}
		}

		this->hide();
	});

	//////////////
	createButton("Open pack", "pack", [&]{

		anax::Entity e = gameScreen->getEntFactory().getPlayer();
		MountComponent& mount = e.getComponent<MountComponent>();

		gameScreen->getManager()->getGui()->get<MountInvenWindow>("mount_window")->
				show(mount.getMount());

		this->hide();
	});

	//////////////
	createButton("Shop", "shop_window", [&]{
		this->hide();

		AiComponent& ai = gameScreen->getEntFactory().getPlayer().getComponent<AiComponent>();

		for(EntityRefNode& ref : gameScreen->getOverworld()->getActiveZone()->zone->getEntityLayer().getEntitiesAt(clickX, clickY)){
			anax::Entity e = gameScreen->getWorld()->getEntity(ref.id);
			if(e.hasComponent<ShopComponent>()){
				ai.board->SetString("current_task", "tgt_shop");
				ai.board->SetVector("tgt_tile", sf::Vector2i(clickX, clickY));
				ai.board->SetInt("tgt_entity", ref.id);
				ai.board->SetBool("needsInput", false);
				return;
			}
		}
	});

	//////////////
	createButton("Attack", "atk_tgt", [&]{
		this->hide();

		AiComponent& ai = gameScreen->getEntFactory().getPlayer().getComponent<AiComponent>();

		for(EntityRefNode& ref : gameScreen->getOverworld()->getActiveZone()->zone->getEntityLayer().getEntitiesAt(clickX, clickY)){
			anax::Entity e = gameScreen->getWorld()->getEntity(ref.id);
			if(e.hasComponent<HealthComponent>()){
				ai.board->SetString("current_task", "atk_tgt");
				ai.board->SetInt("tgt_entity", ref.id);
				ai.board->SetBool("needsInput", false);
				return;
			}
		}
	});

	//////////////
	createButton("Enter portal", "zone_to_overworld", [&]{
		this->hide();

		anax::Entity p = gameScreen->getEntFactory().getPlayer();
		PositionComponent& pos = p.getComponent<PositionComponent>();
		Overworld* overworld = gameScreen->getOverworld();

		//loop entities at position on entitylayer
		for(auto& entRef : overworld->getActiveZone()->zone->getEntityLayer().getEntitiesAt(
				pos.tilePosition.x, pos.tilePosition.y)){

			anax::Entity ent = gameScreen->getWorld()->getEntity(entRef.id);

			//check is entity is a portal
			if(!ent.hasComponent<PortalComponent>()) continue;

			PortalComponent& portal = ent.getComponent<PortalComponent>();

			//remove player from previous zone
			overworld->getActiveZone()->zone->getEntityLayer().removeEntity(pos.tilePosition.x,
					pos.tilePosition.y, p.getId().index);

			if(portal.destZoneName.compare("overworld.save") == 0){
				//move to overworld
				overworld->setChildActive(false);

				ZoneComponent& z = ent.getComponent<ZoneComponent>();

				//get current zone portal
				int exitId = overworld->getActiveZone()->zone->getEntityLayer().getPortalId(z.zoneName);
				anax::Entity exit = gameScreen->getWorld()->getEntity(exitId);
				PositionComponent& exitPos = exit.getComponent<PositionComponent>();

				//place player in the overworld at portal position
				overworld->getActiveZone()->zone->getEntityLayer().setEntity(
						exitPos.tilePosition.x, exitPos.tilePosition.y, p);
				pos.setPosition(exitPos.tilePosition.x, exitPos.tilePosition.y,
						exitPos.screenPosition.x, exitPos.screenPosition.y);

				overworld->unloadChildZone();
				break;
			}
		}
	});

	//////////////
	createButton("Walk to", "walk_overworld", [&]{
		AiComponent& ai = gameScreen->getEntFactory().getPlayer().getComponent<AiComponent>();

		ai.board->SetVector("tgt_tile", sf::Vector2i(clickX, clickY));
		ai.board->SetBool("needsInput", false);

		this->hide();
	});

	//////////////
	createButton("Scout", "scout_overworld", [&]{
		this->hide();
	});

	//////////////
	createButton("Wait turn", "wait_overworld", [&]{
		this->hide();
	});

	//////////////
	createButton("Enter zone", "overworld_to_zone", [&]{
		this->hide();

		//return game speed to normal
		for(auto& w : this->getParent()->get<tgui::Panel>("game_panel")->getWidgets()){
			if(w->getWidgetType().compare("ToggleButton") == 0){
				std::dynamic_pointer_cast<tgui::ToggleButton>(w)->setChecked(false);
			}
		}
		AiSystem::setSpeed(1);

		anax::Entity p = gameScreen->getEntFactory().getPlayer();
		PositionComponent& pos = p.getComponent<PositionComponent>();
		Overworld* overworld = gameScreen->getOverworld();

		//remove player from previous zone
		overworld->getActiveZone()->zone->getEntityLayer().removeEntity(pos.tilePosition.x,
				pos.tilePosition.y, p.getId().index);

		//loop entities at position on entitylayer
		for(auto& entRef : overworld->getActiveZone()->zone->getEntityLayer().getEntitiesAt(
				pos.tilePosition.x, pos.tilePosition.y)){

			anax::Entity ent = gameScreen->getWorld()->getEntity(entRef.id);

			//check is entity is a portal
			if(!ent.hasComponent<PortalComponent>()) continue;

			PortalComponent& portal = ent.getComponent<PortalComponent>();

			std::string zonePath = getenv("HOME");
			zonePath += "/linbobGame/game/";
			overworld->loadChildZone(zonePath + portal.destZoneName, &gameScreen->getEntFactory());

			break;
		}

		overworld->saveZone();
		overworld->setChildActive();

		//get overworld exit portal
		int exitId = overworld->getActiveZone()->zone->getEntityLayer().getPortalId("overworld.save");
		anax::Entity exit = gameScreen->getWorld()->getEntity(exitId);
		PositionComponent& exitPos = exit.getComponent<PositionComponent>();

		//place player in zone at overworld portal position
		overworld->getActiveZone()->zone->getEntityLayer().setEntity(
				exitPos.tilePosition.x, exitPos.tilePosition.y, p);
		pos.setPosition(exitPos.tilePosition.x, exitPos.tilePosition.y,
				exitPos.screenPosition.x, exitPos.screenPosition.y);
	});

	//////////////

	hide();
}
void PlayerContext::update(sf::Time elapsedTime){
	if(isVisible()){
		sf::Vector2i mousePos = sf::Mouse::getPosition(*gameScreen->getManager()->getWindow());
		sf::FloatRect area{getPosition().x - (getSize().x * 0.5), getPosition().y - (getSize().y * 0.4),
			getSize().x * 1.5, getSize().y * 1.4};

		if(!area.contains(mousePos.x, mousePos.y)){
			hide();
		}
	}

}
void PlayerContext::show(int x, int y){
	if(!getWidgets().empty()){
		removeAllWidgets();
	}

	clickX = x;
	clickY = y;

	EntityLayer& eLayer = gameScreen->getOverworld()->getActiveZone()->zone->getEntityLayer();

	if(!gameScreen->getOverworld()->isChildActive()){
		int count = 1;
		add(buttons["walk_overworld"]);

		buttons["wait_overworld"]->setPosition(0, (buttonHeight * count++) + 4);
		add(buttons["wait_overworld"]);

		buttons["scout_overworld"]->setPosition(0, (buttonHeight * count++) + 8);
		add(buttons["scout_overworld"]);

		for(EntityRefNode& ref : eLayer.getEntitiesAt(clickX, clickY)){
			anax::Entity e = gameScreen->getWorld()->getEntity(ref.id);

			if(e.hasComponent<PortalComponent>()){
				buttons["overworld_to_zone"]->setPosition(0, (buttonHeight * count++) + 12);
				add(buttons["overworld_to_zone"]);
				break;
			}
		}
	}else{
		add(buttons["walk_to"]);
		int count = 1;
		for(EntityRefNode& ref : eLayer.getEntitiesAt(clickX, clickY)){
			anax::Entity e = gameScreen->getWorld()->getEntity(ref.id);

			if(e.hasComponent<ItemComponent>()){
				ItemComponent& i = e.getComponent<ItemComponent>();

				if(i.hasType(Item::MOUNT)){
					buttons["ride"]->setPosition(0, (buttonHeight * count++) + 4);
					add(buttons["ride"]);
					buttons["claim"]->setPosition(0, (buttonHeight * count++) + 8);
					add(buttons["claim"]);
					buttons["pack"]->setPosition(0, (buttonHeight * count++) + 12);
					add(buttons["pack"]);
				}

				break;
			}
			std::string refName = e.getComponent<NameComponent>().name;
			if(refName.compare("Loot drop") == 0){
				buttons["pick_up"]->setPosition(0, (buttonHeight * count++) + 4);
				add(buttons["pick_up"]);
			}

			if(e.hasComponent<ShopComponent>()){
				buttons["shop_window"]->setPosition(0, (buttonHeight * count++) + 4);
				add(buttons["shop_window"]);
				break;
			}
			if(e.hasComponent<HealthComponent>()){
				buttons["atk_tgt"]->setPosition(0, (buttonHeight * count++) + 4);
				add(buttons["atk_tgt"]);
				break;
			}
			if(e.hasComponent<PortalComponent>()){
				buttons["zone_to_overworld"]->setPosition(0, (buttonHeight * count++) + 4);
				add(buttons["zone_to_overworld"]);
				break;
			}
		}
	}
	tgui::Widget::show();
}
