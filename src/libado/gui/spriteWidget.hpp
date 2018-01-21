/*
 * spriteWidget.hpp
 *
 *  Created on: 29 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_SPRITEWIDGET_HPP_
#define LIBADO_GUI_SPRITEWIDGET_HPP_

#include <memory>
#include <TGUI/TGUI.hpp>

class SpriteWidget : public tgui::Widget{
public:
	typedef std::shared_ptr<SpriteWidget> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const SpriteWidget> ConstPtr; ///< Shared constant widget pointer

	SpriteWidget() : tgui::Widget(), sprite(new sf::Sprite()){}

	SpriteWidget(sf::Sprite* s) : sprite(new sf::Sprite()){
		sprite->setTexture(*s->getTexture());
		sprite->setTextureRect(s->getTextureRect());
	}
	SpriteWidget(const SpriteWidget& s) : sprite(new sf::Sprite()){
		sprite->setTexture(*s.sprite->getTexture());
		sprite->setTextureRect(s.sprite->getTextureRect());
	}

	virtual bool mouseOnWidget(float x, float y) const{return false;}
	virtual tgui::Widget::Ptr clone() const override{
		return std::make_shared<SpriteWidget>(*this);
	}
	virtual void setPosition(const tgui::Layout2d& position) override{
		sprite->setPosition(position.x.getValue(), position.y.getValue());
	}
	virtual void setPosition(const tgui::Layout& x, const tgui::Layout& y){
		setPosition(tgui::Layout2d(x, y));
	}
	void setSprite(sf::Sprite* s){
		if(!s){
			sprite.reset(new sf::Sprite());
		}else{
			sprite->setTexture(*s->getTexture());
			sprite->setTextureRect(s->getTextureRect());
		}
	}
	sf::Sprite* getSprite(){
		return sprite.get();
	}
protected:
	virtual void update(sf::Time elapsedTime) override{}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override{
		target.draw(*sprite, states);
	}
private:
	std::unique_ptr<sf::Sprite> sprite;
};

#endif /* LIBADO_GUI_SPRITEWIDGET_HPP_ */
