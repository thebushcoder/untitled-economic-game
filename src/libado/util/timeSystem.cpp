/*
 * timeSystem.cpp
 *
 *  Created on: 28 Feb 2017
 *      Author: linbob
 */

#include "timeSystem.hpp"

void TimeSystem::update(){
	++totalTicks;

	time += hrPerTick; //1 production tick = 3 hours

	if(time >= tickPerDay * hrPerTick){
		time = 0;
		++day;
	}
	if(day >= daysPerWeek * weeksPerMonth){
		++month;
		day = 0;
	}
	if(month >= monthPerYr){
		++year;
		month = 0;
	}
	updateWidget();
}
void TimeSystem::updateWidget(){
	if(timeWidget){
		timeWidget->get<tgui::Label>("time")->setText(std::to_string(time) + " hr");
		timeWidget->get<tgui::Label>("day")->setText(std::to_string(day));
		timeWidget->get<tgui::Label>("month")->setText(std::to_string(month));
		timeWidget->get<tgui::Label>("year")->setText("Yr : " + std::to_string(year));
	}
}
void TimeSystem::load(rapidjson::Value& timeData){
	totalTicks = timeData["tickTotal"].GetInt();
	time = timeData["time"].GetInt();
	day = timeData["day"].GetInt();
	month = timeData["month"].GetInt();
	year = timeData["year"].GetInt();

	updateWidget();
}
void TimeSystem::save(rapidjson::Document& json){
	rapidjson::Value timeJSON(rapidjson::kObjectType);

	timeJSON.AddMember("tickTotal", totalTicks, json.GetAllocator());
	timeJSON.AddMember("time", time, json.GetAllocator());
	timeJSON.AddMember("day", day, json.GetAllocator());
	timeJSON.AddMember("month", month, json.GetAllocator());
	timeJSON.AddMember("year", year, json.GetAllocator());

	json.AddMember("TimeSys", timeJSON, json.GetAllocator());
}
TimeSystem* TimeSystem::getInstance(){
	static TimeSystem timeSys;
	return &timeSys;
}
