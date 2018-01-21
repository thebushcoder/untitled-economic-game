/*
 * spriteButton.cpp
 *
 *  Created on: 22 Sep 2016
 *      Author: linbob
 */

#include "spriteButton.hpp"

SpriteButton::SpriteButton() : tgui::Button(){}

void SpriteButton::setSprite(std::unique_ptr<sf::Sprite> s){
	sprite = std::move(s);
	sprite->setPosition(getSize().x * 0.06 + getPosition().x + sprite->getGlobalBounds().width / 2,
			getSize().y * 0.06 + getPosition().y + sprite->getGlobalBounds().height / 2);
}
void SpriteButton::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	tgui::Button::draw(target, states);
	target.draw(*sprite, states);
}
void SpriteButton::setPosition(const tgui::Layout2d& position){
	tgui::Button::setPosition(position);
	if(sprite){
		sprite->setPosition(getSize().x * 0.06 + getPosition().x + sprite->getGlobalBounds().width / 2,
				getSize().y * 0.06 + getPosition().y + sprite->getGlobalBounds().height / 2);
	}
}
void SpriteButton::setPosition(const tgui::Layout& x, const tgui::Layout& y){
	setPosition(tgui::Layout2d(x, y));
}
