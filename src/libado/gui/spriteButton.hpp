/*
 * spriteButton.hpp
 *
 *  Created on: 22 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_SPRITEBUTTON_HPP_
#define LIBADO_GUI_SPRITEBUTTON_HPP_

#include <TGUI/TGUI.hpp>

class SpriteButton : public tgui::Button{
public:
	typedef std::shared_ptr<SpriteButton> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const SpriteButton> ConstPtr; ///< Shared constant widget pointer

	SpriteButton();
	void setSprite(std::unique_ptr<sf::Sprite> s);
	sf::Sprite& getSprite(){
		return * sprite;
	}
	virtual void setPosition(const tgui::Layout2d& position) override;
	virtual void setPosition(const tgui::Layout& x, const tgui::Layout& y);
protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	std::unique_ptr<sf::Sprite> sprite;
};



#endif /* LIBADO_GUI_SPRITEBUTTON_HPP_ */
