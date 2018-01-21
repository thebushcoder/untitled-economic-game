/*
 * SpriteComponent.hpp
 *
 *  Created on: 26 Feb 2016
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_

#include <anax/Component.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../../textureAtlas.hpp"

struct SpriteComponent : anax::Component{
	std::unique_ptr<sf::Sprite> sprite;

	SpriteComponent(rapidjson::Value& jsonData, TextureAtlas& atlas){
		sprite = atlas.createSprite(jsonData.GetString());
	}
	SpriteComponent(std::string name, TextureAtlas& atlas){
		sprite = atlas.createSprite(name);
	}
};

#endif /* LIBADO_ENTITY_COMPONENTS_SPRITECOMPONENT_HPP_ */
