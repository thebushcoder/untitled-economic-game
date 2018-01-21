/*
 * cutCopyArea.cpp
 *
 *  Created on: 13 Oct 2016
 *      Author: linbob
 */

#include "areaEdit.hpp"

AreaEdit::AreaEdit(WorldView* w_view, EntityFactory* entFactory, TileMap* map) :
	map(map), entFactory(entFactory), w_view(w_view){
	areaSprite = map->getFactory()->getAtlas().createSprite("gui/magicmap");
}
void AreaEdit::setActive(Mode mode, bool active){
	isActive = active;
	this->mode = mode;
	clickCount = 0;
	area = sf::IntRect();
}
bool AreaEdit::leftClick(WorldView* w_view, sf::RenderWindow* window){
	if(!isActive) return false;
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2f worldVec = window->mapPixelToCoords(mousePos, *(w_view->view));

	//tile position
	int x = std::round(worldVec.x / TILESIZE);
	int y = std::round(worldVec.y / TILESIZE);

	//check map bounds
	if(x < 0 || y < 0 || x >= map->getWidth() || y >= map->getHeight()) return false;

	switch(clickCount){
	case 0:
		// select area start (x,y)
		area.left = x;
		area.top = y;
		++clickCount;
		break;
	case 1:
		// select area end (x,y) - square's width/height
		if(mode == Mode::DELETE){
			int areaX = std::min(area.left, x);
			int areaY = std::min(area.top, y);
			int areaW = std::abs(x - area.left);
			int areaH = std::abs(y - area.top);

			area.left = areaX;
			area.top = areaY;
			area.width = areaW;
			area.height = areaH;

			deleteArea();
			isActive = false;
			return true;
		}else if(x != area.left && y != area.top){
			int areaX = std::min(area.left, x);
			int areaY = std::min(area.top, y);
			int areaW = std::abs(x - area.left);
			int areaH = std::abs(y - area.top);

			area.left = areaX;
			area.top = areaY;
			area.width = areaW;
			area.height = areaH;

			++clickCount;
		}
		break;
	case 2:
		// destination to apply action's square area
		clickX = x;
		clickY = y;

		switch(mode){
		case CUT:
			cutArea();
			break;
		case COPY:
			copyArea();
			break;
		}
		isActive = false;
		break;
	default:
		return false;
		break;
	}
	return true;
}
bool AreaEdit::rightClick(){
	if(isActive){
		isActive = false;
		return true;
	}
	return false;
}
void AreaEdit::render(sf::RenderWindow& window){
	if(!isActive) return;
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f worldVec = window.mapPixelToCoords(mousePos, *(w_view->view));

	//tile position
	int x = std::round(worldVec.x / TILESIZE);
	int y = std::round(worldVec.y / TILESIZE);

	//check map bounds
	if(x < 0 || y < 0 || x >= map->getWidth() || y >= map->getHeight()) return;

	switch(clickCount){
	case 0:
	{
		areaSprite->setPosition(x * TILESIZE, y * TILESIZE);
		window.draw(*areaSprite);
		break;
	}
	case 1:
	{
		int areaX = std::min(area.left, x);
		int areaY = std::min(area.top, y);
		int areaW = std::abs(x - area.left);
		int areaH = std::abs(y - area.top);

		//draw area preview
		for(int renderX = areaX; renderX <= areaX + areaW; ++renderX){
			for(int renderY = areaY; renderY <= areaY + areaH; ++renderY){
				sf::Sprite sprite = sf::Sprite(*areaSprite->getTexture(), areaSprite->getTextureRect());

				sprite.setPosition(renderX * TILESIZE - (sprite.getGlobalBounds().width / 2),
						renderY * TILESIZE - (sprite.getGlobalBounds().height / 2));

				window.draw(sprite);
			}
		}
		break;
	}
	case 2:
	{
		//draw area preview
		for(int renderX = x; renderX <= x + area.width; ++renderX){
			for(int renderY = y; renderY <= y + area.height; ++renderY){
				sf::Sprite sprite = sf::Sprite(*areaSprite->getTexture(), areaSprite->getTextureRect());

				sprite.setPosition(renderX * TILESIZE - (sprite.getGlobalBounds().width / 2),
						renderY * TILESIZE - (sprite.getGlobalBounds().height / 2));

				window.draw(sprite);
			}
		}
		break;
	}
	}
}
// loop area > load TileVec & entityVec, loop area > unload TileVec & entityVec
void AreaEdit::copyArea(){
	std::vector<std::vector<std::vector<std::unique_ptr<Tile>>> > tileCopies;
	tileCopies.resize((area.width + 1));
	for (int i = 0; i <= area.width; ++i){
		tileCopies[i].resize(area.height + 1);
	}
	for(int x = 0; x <= area.width; ++x){
		for(int y = 0; y <= area.height; ++y){
			tileCopies[x][y] = std::vector<std::unique_ptr<Tile>>();
		}
	}
	std::vector<std::vector<std::vector<EntityRefNode>> > entityCopies;
	entityCopies.resize(area.width + 1);
	for (int i = 0; i <= area.width; ++i){
		entityCopies[i].resize(area.height + 1);
	}

	for(int renderX = 0, destX = clickX; renderX <= area.width; ++renderX, ++destX){
		for(int renderY = 0, destY = clickY; renderY <= area.height; ++renderY, ++destY){
			for(auto& t : map->getTileLayer().getTile(area.left + renderX,
					area.top + renderY)){
				if(!t) continue;

				tileCopies[renderX][renderY].push_back(std::unique_ptr<Tile>(new Tile()));
				tileCopies[renderX][renderY].back()->copyTile(t.get());
			}
			entityCopies[renderX][renderY] = map->getEntityLayer().getEntitiesAt(
					area.left + renderX, area.top + renderY);
		}
	}
	for(int renderX = 0, destX = clickX; renderX <= area.width; ++renderX, ++destX){
		for(int renderY = 0, destY = clickY; renderY <= area.height; ++renderY, ++destY){
			//copy tile data
			for(auto& t : tileCopies[renderX][renderY]){
				map->getTileLayer().setTile(destX, destY, std::move(t));

				Tile* destTile = map->getTileLayer().getTile(destX, destY).back().get();
				destTile->setPosition(destX * TILESIZE - (destTile->getTileSprite().getGlobalBounds().height / 2),
						destY * TILESIZE - (destTile->getTileSprite().getGlobalBounds().height / 2));
			}



			//copy entity data
			std::vector<EntityRefNode> srcList = entityCopies[renderX][renderY];
			for(EntityRefNode& ref: srcList){
				anax::Entity srcEntity = entFactory->world->getEntity(ref.id);
				srcEntity.getComponent<PositionComponent>().setPosition(destX,
						destY, destX * TILESIZE, destY * TILESIZE);
				map->getEntityLayer().setEntity(destX, destY, srcEntity);
			}
		}
	}
}
void AreaEdit::cutArea(){
	std::vector<std::vector<std::vector<std::unique_ptr<Tile>>> > tileCopies;
	tileCopies.resize((area.width + 1));
	for (int i = 0; i <= area.width; ++i){
		tileCopies[i].resize(area.height + 1);
	}
	for(int x = 0; x <= area.width; ++x){
		for(int y = 0; y <= area.height; ++y){
			tileCopies[x][y] = std::vector<std::unique_ptr<Tile>>();
		}
	}
	std::vector<std::vector<std::vector<EntityRefNode>> > entityCopies;
	entityCopies.resize(area.width + 1);
	for (int i = 0; i <= area.width; ++i){
		entityCopies[i].resize(area.height + 1);
	}

	for(int renderX = 0, destX = clickX; renderX <= area.width; ++renderX, ++destX){
		for(int renderY = 0, destY = clickY; renderY <= area.height; ++renderY, ++destY){
			for(auto& t : map->getTileLayer().getTile(area.left + renderX,
					area.top + renderY)){
				if(!t) continue;

				tileCopies[renderX][renderY].push_back(std::unique_ptr<Tile>(new Tile()));
				tileCopies[renderX][renderY].back()->copyTile(t.get());

				//remove src tile
				map->getTileLayer().deleteTile(area.left + renderX, area.top + renderY);
			}

			entityCopies[renderX][renderY] = map->getEntityLayer().getEntitiesAt(
					area.left + renderX, area.top + renderY);
		}
	}

	for(int renderX = 0, destX = clickX; renderX <= area.width; ++renderX, ++destX){
		for(int renderY = 0, destY = clickY; renderY <= area.height; ++renderY, ++destY){
			//copy tile data
			for(auto& t : tileCopies[renderX][renderY]){
				map->getTileLayer().setTile(destX, destY, std::move(t));

				Tile* destTile = map->getTileLayer().getTile(destX, destY).back().get();
				destTile->setPosition(destX * TILESIZE - (destTile->getTileSprite().getGlobalBounds().height / 2),
						destY * TILESIZE - (destTile->getTileSprite().getGlobalBounds().height / 2));
			}

			//copy entity data
			std::vector<EntityRefNode> srcList = entityCopies[renderX][renderY];
			for(EntityRefNode& ref: srcList){
				anax::Entity srcEntity = entFactory->world->getEntity(ref.id);
				srcEntity.getComponent<PositionComponent>().setPosition(destX,
						destY, destX * TILESIZE, destY * TILESIZE);
				map->getEntityLayer().setEntity(destX, destY, srcEntity);
				//remove src entity
				map->getEntityLayer().removeEntity(area.left + renderX, area.top +
						renderY, srcEntity.getId().index);
			}
		}
	}
}
void AreaEdit::deleteArea(){
	for(int renderX = area.left, destX = clickX; renderX <= area.left + area.width; ++renderX, ++destX){
		for(int renderY = area.top, destY = clickY; renderY <= area.top + area.height; ++renderY, ++destY){
			//remove src tile
			map->getTileLayer().deleteTile(renderX, renderY);

			//copy entity data
			std::vector<EntityRefNode> srcList = map->getEntityLayer().getEntitiesAt(renderX, renderY);
			for(EntityRefNode& ref: srcList){
				anax::Entity srcEntity = entFactory->world->getEntity(ref.id);
				//remove src entity
				map->getEntityLayer().removeEntity(renderX, renderY, srcEntity.getId().index);
				srcEntity.kill();
			}
		}
	}
}
