/*
 * timeSystem.hpp
 *
 *  Created on: 28 Feb 2017
 *      Author: linbob
 */

#ifndef LIBADO_UTIL_TIMESYSTEM_HPP_
#define LIBADO_UTIL_TIMESYSTEM_HPP_

#include <TGUI/TGUI.hpp>

#include <rapidjson/document.h>

class TimeSystem{
	int totalTicks = 0;
	int time, day, month, year = 0;

	static constexpr int hrPerTick = 3;
	static constexpr int tickPerDay = 7;

	static constexpr int daysPerWeek = 5;
	static constexpr int weeksPerMonth = 4;
	static constexpr int monthPerYr = 8;

	tgui::Panel::Ptr timeWidget;

public:
	TimeSystem(){}

	int getTime(){
		return time;
	}
	int getDay(){
		return day;
	}
	int getMonth(){
		return month;
	}
	int getYear(){
		return year;
	}
	void update();
	void updateWidget();
	void load(rapidjson::Value& timeData);
	void save(rapidjson::Document& json);
	void setWidget(tgui::Panel::Ptr t){
		timeWidget = t;
	}
	static TimeSystem* getInstance();
};

#endif /* LIBADO_UTIL_TIMESYSTEM_HPP_ */
