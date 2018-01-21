/*
 * tooltip.hpp
 *
 *  Created on: 24 Sep 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_TOOLTIP_HPP_
#define LIBADO_GUI_TOOLTIP_HPP_

#include <TGUI/TGUI.hpp>
#include <SFML/Window.hpp>

#include <anax/World.hpp>

#include "../screenManager.hpp"
#include "../map/tileMap.hpp"
#include "../worldView.hpp"

class TooltipAnchor : public tgui::Widget{
public:
	typedef std::shared_ptr<TooltipAnchor> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const TooltipAnchor> ConstPtr; ///< Shared constant widget pointer

	constexpr static float delay = 0.5f;

	TooltipAnchor(TileMap* map, anax::World* world, WorldView* w_view,
			ScreenManager* manager);
	TooltipAnchor(const TooltipAnchor& copy);
	void mouseMoved();
	tgui::Label& getTooltip(){
		return *tooltip;
	}
	virtual bool mouseOnWidget(float x, float y) const{return false;}
	virtual tgui::Widget::Ptr clone() const override{
        return std::make_shared<TooltipAnchor>(*this);
    }
protected:
	virtual void update(sf::Time elapsedTime) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override{}
private:
	TileMap* map;
	anax::World* world;
	WorldView* w_view;
	sf::RenderWindow* window;

	tgui::Label::Ptr tooltip;

	int tileX = -1, tileY = -1;
	sf::Vector2i mousePos;
	sf::Time delayCount;
};

#endif /* LIBADO_GUI_TOOLTIP_HPP_ */
