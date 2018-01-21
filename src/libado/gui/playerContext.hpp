/*
 * playerContext.hpp
 *
 *  Created on: 25 Aug 2016
 *      Author: linbob
 */

#ifndef LIBADO_GUI_PLAYERCONTEXT_HPP_
#define LIBADO_GUI_PLAYERCONTEXT_HPP_

#include <TGUI/TGUI.hpp>

#include "../entity/entityFactory.hpp"
#include "../util/aStar.hpp"
#include "../worldView.hpp"
#include "../map/zone.hpp"

#include "toggleButton.hpp"
#include "../entity/systems/AiSystem.hpp"

#include "../entity/components/componentsCollection.hpp"

class GameScreen;
class PlayerContext : public tgui::Panel{
public:
	typedef std::shared_ptr<PlayerContext> Ptr; ///< Shared widget pointer
	typedef std::shared_ptr<const PlayerContext> ConstPtr; ///< Shared constant widget pointer

	PlayerContext(GameScreen* gs);

	virtual void show(int x, int y);
protected:
	virtual void update(sf::Time elapsedTime) override;
	template <typename Func>
	void createButton(std::string label, std::string tag, Func func){
		tgui::Button::Ptr button = theme->load("Button");
		button->setText(label);
		button->setSize(getSize().x, buttonHeight);
		button->connect("pressed", func);
		buttons[tag] = button;
	}
private:
	GameScreen* gameScreen;
	tgui::Theme::Ptr theme;

	int clickX, clickY;
	int buttonHeight = 28;
	std::map<std::string, tgui::Button::Ptr> buttons;
};

#endif /* LIBADO_GUI_PLAYERCONTEXT_HPP_ */
