/*
 * townTooltip.hpp
 *
 *  Created on: 17 Feb 2017
 *      Author: linbob
 */

#ifndef LIBADO_GUI_TOWNTOOLTIP_HPP_
#define LIBADO_GUI_TOWNTOOLTIP_HPP_

#include <TGUI/TGUI.hpp>
#include <SFML/Window.hpp>
#include <iomanip> // setprecision
#include <sstream>

#include <anax/World.hpp>

#include "../screenManager.hpp"
#include "../map/tileMap.hpp"
#include "../worldView.hpp"

class TownTooltip : public tgui::Widget{
public:
	typedef std::shared_ptr<TownTooltip> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const TownTooltip> ConstPtr; ///< Shared constant widget pointer

	constexpr static float delay = 0.5f;

	TownTooltip(TileMap* map, anax::World* world, WorldView* w_view,
			ScreenManager* manager);
	TownTooltip(const TownTooltip& copy);

	void mouseMoved();
	virtual bool mouseOnWidget(float x, float y) const{return false;}
	virtual tgui::Widget::Ptr clone() const override{
        return std::make_shared<TownTooltip>(*this);
    }
protected:
	virtual void update(sf::Time elapsedTime) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override{}
private:
	TileMap* map;
	anax::World* world;
	WorldView* w_view;
	sf::RenderWindow* window;

	tgui::Panel::Ptr tooltip;
	int entityId = -1;

	int tileX = -1, tileY = -1;
	sf::Vector2i mousePos;
	sf::Time delayCount;
};

#endif /* LIBADO_GUI_TOWNTOOLTIP_HPP_ */
