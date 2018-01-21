/*
 * timeWidget.cpp
 *
 *  Created on: 2 Mar 2017
 *      Author: linbob
 */

#include "timeWidget.hpp"

tgui::Panel::Ptr TimeWidget::createTimeWidget(tgui::Theme::Ptr theme){
		float w = 32, h = 25;
		tgui::Panel::Ptr panel = std::make_shared<tgui::Panel>();
		panel->setSize(w * 4.1, h * 1.1);

		tgui::Label::Ptr time = theme->load("Label");
		time->setTextSize(12);
		time->setText("0 hr");
		time->setSize(w, h);
		time->setTextColor(sf::Color::Black);
		panel->add(time, "time");

		tgui::Label::Ptr day = theme->load("Label");
		day->setTextSize(12);
		day->setText("0");
		day->setSize(w, h);
		day->setTextColor(sf::Color::Black);
		panel->add(day, "day");
		day->setPosition(w, 0);

		tgui::Label::Ptr month = theme->load("Label");
		month->setTextSize(12);
		month->setSize(w, h);
		month->setTextColor(sf::Color::Black);
		month->setText("0");
		panel->add(month, "month");
		month->setPosition(w * 2, 0);

		tgui::Label::Ptr year = theme->load("Label");
		year->setTextSize(12);
		year->setSize(w, h);
		year->setTextColor(sf::Color::Black);
		year->setText("0");
		panel->add(year, "year");
		year->setPosition(w * 3, 0);

		return panel;
}
