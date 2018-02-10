/*
 * river.cpp
 *
 *  Created on: 6 Feb 2018
 *      Author: linbob
 */

#include "cellcorner.hpp"
#include "river.hpp"

namespace VoronoiMap{
		std::vector<std::shared_ptr<CellCorner>> River::getCornerPath(CellCorner* src, CellCorner* dest){
			std::vector<std::shared_ptr<CellCorner>> list;

			if(isUpstream(src, dest)){
				std::shared_ptr<CellCorner> up = src->getUpstream();
				list.push_back(up->getDownStream());
				while(up){
					list.push_back(up);
					up = up->getUpstream();
				}
			}else if(isDownstream(src, dest)){
				std::shared_ptr<CellCorner> down = src->getDownStream();
				list.push_back(down->getUpstream());
				while(down){
					list.push_back(down);
					down = down->getDownStream();
				}
			}

			return list;
		}
		bool River::isUpstream(CellCorner* src, CellCorner* dest){
			CellCorner* up = src->getUpstream().get();
			while(up){
				if(up == dest){
					return true;
				}
				up = up->getUpstream().get();
			}
			return false;
		}
		bool River::isDownstream(CellCorner* src, CellCorner* dest){
			CellCorner* down = src->getDownStream().get();
			while(down){
				if(down == dest){
					return true;
				}
				down = down->getDownStream().get();
			}
			return false;
		}
}
