/*
 * tileLayer.cpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#include "tileLayer.hpp"

TileLayer::TileLayer(int width, int height){
	map.resize(width);
	for (int i = 0; i < width; ++i){
		TileLayer::map[i].resize(height);
	}
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			map[x][y] = std::vector<std::unique_ptr<Tile>>();
		}
	}
}
void TileLayer::resize(int w, int h){
	int wDiff = w - map.size();
	int hDiff = h - map[0].size();

	map.resize(w);
	for (int i = 0; i < w; ++i){
		TileLayer::map[i].resize(h);
	}

	for(int x = map.size() - wDiff; x < map.size(); ++x){
		for(int y = map[0].size() - hDiff; y < map[x].size(); ++y){
			map[x][y] = std::vector<std::unique_ptr<Tile>>();
		}
	}
}
void TileLayer::load(rapidjson::Value& layerData, TileFactory& factory){
	for(int j = 0; j < layerData.Size(); ++j){
		int x = layerData[j]["x"].GetInt();
		int y = layerData[j]["y"].GetInt();
		int id = layerData[j]["id"].GetInt();
		setTile(x, y, factory.createTile(id));
		if(map[x][y].size() == 1){
			map[x][y][0]->setPosition(x * TILESIZE, y * TILESIZE);
		}else if(map[x][y].size() == 2){
			map[x][y][1]->setPosition(x * TILESIZE, y * TILESIZE);
		}
	}
}
void TileLayer::save(rapidjson::Value& layerData,
		rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc){
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			if(map[x][y].empty()) continue;
			for(int i = 0; i < map[x][y].size(); ++i){
				if(!map[x][y][i]) continue;
				rapidjson::Value t(rapidjson::kObjectType);
				t.AddMember("x", x, dAlloc);
				t.AddMember("y", y, dAlloc);
				t.AddMember("id", map[x][y][i]->getId(), dAlloc);

				layerData.PushBack(t, dAlloc);
			}
		}
	}
}
void TileLayer::render(sf::RenderWindow* window){
	for(int x = 0; x < map.size(); ++x){
		for(int y = 0; y < map[x].size(); ++y){
			if(map[x][y].empty()){
				sf::RectangleShape outline;
				outline.setSize(sf::Vector2f(TILESIZE, TILESIZE));
				outline.setFillColor(sf::Color::Transparent);
				outline.setOutlineColor(sf::Color::Magenta);
				outline.setOutlineThickness(1.2f);
				outline.setPosition(x * TILESIZE - (TILESIZE / 2),
						y * TILESIZE - (TILESIZE / 2));

				window->draw(outline);
				continue;
			}
			//RENDER TILE
			for(int i = 0; i < map[x][y].size(); ++i){
				if(!map[x][y][i]) continue;
				window->draw(map[x][y][i]->getTileSprite());
			}
			//debug dots
			sf::RectangleShape point;
			point.setSize(sf::Vector2f(2, 2));
			point.setFillColor(sf::Color::Magenta);
			point.setPosition(x * TILESIZE, y * TILESIZE);

			window->draw(point);
		}
	}
}
int TileLayer::getWidth(){
	return map.size();
}
int TileLayer::getHeight(){
	return map[0].size();
}
bool TileLayer::isOccupied(int x, int y){
	return !map[x][y].empty();
}
std::vector<std::unique_ptr<Tile>>& TileLayer::getTile(int x, int y){
	return map[x][y];
}
void TileLayer::setTile(int x, int y, std::unique_ptr<Tile> t){
	if(t->getId() < FOREGROUND_TILE){
		if(map[x][y].empty()){
			map[x][y].push_back(std::move(t));
		}else{
			map[x][y][0].reset(t.release());
		}
	}else if(t->getId() >= FOREGROUND_TILE){
		if(map[x][y].size() < 2){
			map[x][y].push_back(std::move(t));
		}else{
			map[x][y][1].reset(t.release());
		}
	}
}
void TileLayer::deleteTile(int x, int y){
	if(map[x][y].empty()) return;
	map[x][y].pop_back();
//	if(map[x][y].size() == 2){
//		map[x][y][1] = nullptr;
//	}else if(map[x][y].size() == 1){
//		map[x][y][0] = nullptr;
//	}
}
