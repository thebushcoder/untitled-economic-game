/*
 * tileLayer.hpp
 *
 *  Created on: 28 Mar 2016
 *      Author: linbob
 */

#ifndef LIBADO_MAP_TILELAYER_HPP_
#define LIBADO_MAP_TILELAYER_HPP_

#include <vector>
#include <memory>
#include <SFML/Window.hpp>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "tileFactory.hpp"
#include "layer.hpp"

class TileLayer : Layer{
public:
	static constexpr int FOREGROUND_TILE = 100;

	TileLayer(){}
	TileLayer(int width, int height);

	void load(rapidjson::Value& layerData, TileFactory& factory);
	void save(rapidjson::Value& layerData,
			rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& dAlloc);

	virtual void render(sf::RenderWindow* window);
	virtual void resize(int w, int h);
	virtual int getWidth();
	virtual int getHeight();
	virtual bool isOccupied(int x, int y);
	std::vector<std::unique_ptr<Tile>>& getTile(int x, int y);
	void setTile(int x, int y, std::unique_ptr<Tile> t);
	void setTile(int x, int y, Tile* t);
	void deleteTile(int x, int y);
protected:
	std::vector<std::vector<std::vector<std::unique_ptr<Tile>>> > map;
};

#endif /* LIBADO_MAP_TILELAYER_HPP_ */
