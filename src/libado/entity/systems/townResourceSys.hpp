/*
 * townResourcesSys.hpp
 *
 *  Created on: 2 Jan 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_SYSTEMS_TOWNRESOURCESYS_HPP_
#define LIBADO_ENTITY_SYSTEMS_TOWNRESOURCESYS_HPP_

#include <iomanip> // setprecision
#include <sstream>
#include <anax/System.hpp>

#include "../../util/bt-master/bt.hpp"
#include "../components/productionComponent.hpp"
#include "../components/consumptionComponent.hpp"
#include "../components/resourcesComponent.hpp"
#include "../components/populationComponent.hpp"
#include "../components/stackableComponent.hpp"
#include "../components/nameComponent.hpp"
#include "../components/itemComponent.hpp"
#include "../components/valueComponent.hpp"

#include "../../util/fileLogger.hpp"
using namespace Logging;

struct TownResourceSys : public anax::System<anax::Requires<ProductionComponent, ConsumptionComponent, ResourcesComponent>>{
	const float nonWkrPop = 0.3f; //% of population who are non-workers
	static constexpr float resourceConsump = 0.95f;
	static constexpr float wkrFoodConsump = 2;
	static constexpr float nonwkrFoodConsump = 1;
	static constexpr int dailyProductionTicks = 7;
	static constexpr float optimalSupply = 2.3f; //2 1/2 weeks supply

	std::pair<float, float> maxPriceIncrease; //weeks supply, percent diff from base price
	std::pair<float, float> maxPriceDecrease; //weeks supply, percent diff from base price

	void process(){
		for(anax::Entity e : getEntities()){
			ProductionComponent& prod = e.getComponent<ProductionComponent>();
			PopulationComponent& pop = e.getComponent<PopulationComponent>();
			ConsumptionComponent& consump = e.getComponent<ConsumptionComponent>();
			ResourcesComponent& res = e.getComponent<ResourcesComponent>();

			float workersTotal = pop.value - pop.value * nonWkrPop;

			NameComponent& n = e.getComponent<NameComponent>();
			FileLogger::getInstance()->logString("!!!TownResourceSys : UPDATE - " + n.name + "\n", "resourcesDebug");

			//produce resources
			for(auto& r : prod.producedResources){
				float workers = workersTotal * r.second.second;
				float prodQty = r.second.first.first;
				if(prodQty == 0){
					prodQty = prod.baseProductionRate;
				}
				float productionResult = workers * prodQty;
				NameComponent& n = res.map[r.first].e.getComponent<NameComponent>();
				StackableComponent& stack = res.map[r.first].e.getComponent<StackableComponent>();

				std::stringstream ss;
				ss << std::fixed << std::setprecision(2) << productionResult;

				FileLogger::getInstance()->logString("\t** " + n.name + " ; production qty : " + ss.str() + "\n", "resourcesDebug");

				stack.count += productionResult;
				r.second.first.second = productionResult;
			}

			//consume resources
			for(auto& c : consump.consumedResources){
				float consumeValue = workersTotal * c.mainQty;

				consumeValue += (pop.value * nonWkrPop) * c.secondaryQty;

				std::stringstream ss;
				ss << std::fixed << std::setprecision(2) << consumeValue;

				FileLogger::getInstance()->logString("\t TOTAL consumeValue : " + ss.str() + "\n\n", "resourcesDebug");

				consumeResourceType(res, prod, c.type, consumeValue);
			}

			calculatePrices(res, pop, consump);

			for(auto& r : res.map){
				StackableComponent& stack = r.second.e.getComponent<StackableComponent>();
				std::stringstream ss;
				ss << std::fixed << std::setprecision(2) << r.second.currentPrice;
				FileLogger::getInstance()->logString("\t\t" + r.first + " : " + std::to_string(stack.count) +
						" (QTY), $" + ss.str() + "(PRICE)\n", "resourcesDebug");
			}
		}
	}
	static TownResourceSys* getInstance(){
		static TownResourceSys inst;
		if(inst.maxPriceDecrease.first == 0.0f){
			inst.maxPriceIncrease = std::pair<float, float>(0.3f, 0.6f);
			inst.maxPriceDecrease = std::pair<float, float>(4.3f, -0.5f);
		}
		return &inst;
	}
	/*
	 * Calculate price based on current weeks supply;
	 * over supply = decrease in price per unit to a max of price-50%
	 * under supply = increase in price per unit to a max of price+60%
	 *
	 */
	void calculatePrices(ResourcesComponent& res, PopulationComponent& pop, ConsumptionComponent& consump){
		float workersTotal = pop.value - pop.value * nonWkrPop;
		for(auto& r : res.map){
			float consumeValue = 0.0f;

			//get per production tick consumption of item type
			ItemComponent& i = r.second.e.getComponent<ItemComponent>();
			for(Item::Type t : i.types){
				for(auto& c : consump.consumedResources){
					if(c.type == t){
						consumeValue = workersTotal * c.mainQty;
						consumeValue += (pop.value * nonWkrPop) * c.secondaryQty;
						break;
					}
				}
				if(consumeValue > 0) break;
			}

			NameComponent& n = r.second.e.getComponent<NameComponent>();
			ValueComponent& value = r.second.e.getComponent<ValueComponent>();
			StackableComponent& stack = r.second.e.getComponent<StackableComponent>();

			//current supply / daily consumption
			float currentWeeksSupply = (stack.count / (consumeValue * dailyProductionTicks)) / 10;
			//round to nearest ONE DECIMAL(0.0) place, ie nearest ingame day
			currentWeeksSupply = std::round(currentWeeksSupply * 10) / 10;

			//supply is > than 1 week; adjust weeksSupply to conform to 7 day week
			if(currentWeeksSupply > 0.7f){
				currentWeeksSupply = calculateNumWeeks(currentWeeksSupply);
			}

			r.second.currentWeeksSupply = currentWeeksSupply;

			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << currentWeeksSupply;

			FileLogger::getInstance()->logString("\tCurrent weeks supply of " +
					n.name + " : " + ss.str() + "\n", "resourcesDebug");

			if(currentWeeksSupply > optimalSupply){

				//excess supply
				float price;
				if(currentWeeksSupply >= maxPriceDecrease.first){
					price = value.getValue() + (value.getValue() * maxPriceDecrease.second);
				}else{
					//max - max / days from optimalSupply
					float priceVariance = maxPriceDecrease.second - (maxPriceDecrease.second / ((currentWeeksSupply - optimalSupply) * 10));
					price = value.getValue() + (value.getValue() * priceVariance);
				}
				r.second.currentPrice = price;

			}else if(currentWeeksSupply < optimalSupply){
				//deficit supply
				float price;

				if(currentWeeksSupply <= maxPriceIncrease.first){
					//at or below max threshold
					price = value.getValue() + (value.getValue() * maxPriceIncrease.second);
				}else{
					//number of days from optimal supply
					float daysDiff = getDaysDiff(currentWeeksSupply);

					float p = calculatePercentage(maxPriceIncrease, daysDiff);

					price = value.getValue() + (value.getValue() * p);
				}

				r.second.currentPrice = price;
			}else{
				//optimal supply
				r.second.currentPrice = value.getValue();
			}
		}
	}
	float calculatePercentage(std::pair<float, float>& maxPriceVar, float& daysDiff){
		float p = -100;
		//recursively calculate % price variance until i == current days supply
		for(float i = getDaysDiff(maxPriceVar.first + 0.1); i >= daysDiff - 0.1; i = i - 0.1){
			//initial % calc
			if(p == -100){
				// i * 10 to make float(1.3 daysDif) to int(13 daysDif)
				p = maxPriceVar.second - (maxPriceVar.second / (i * 10));
			}else{
				p = p - (p / (i * 10));
			}
		}
		return p;
	}

	float getDaysDiff(float currentWeeksSupply){
		float daysDiff = optimalSupply - currentWeeksSupply;

		if(currentWeeksSupply <= 0.9){
			daysDiff -= 0.6;
		}else if(currentWeeksSupply <= 1.9){
			daysDiff -= 0.3;
		}

		return daysDiff;
	}
	float calculateNumWeeks(float currentWeeksSupply){
			int weeks = 0;
			float daysCount = 0;

			//std::round(f1 * 10) / 10 = round to nearest single decimal(0.100000)
			for(float c = 0; c < (std::round(currentWeeksSupply * 10) / 10); c += 0.1f){
				daysCount += 0.1f;
				if(daysCount >= 0.7f){
					++weeks;
					daysCount = 0;
				}
			}
			float total = daysCount + weeks;

			return total;
	}

	void consumeResourceType(ResourcesComponent& res, ProductionComponent& prod, Item::Type resType, float& consumeValue){
		float resTotal = 0;
		std::vector<std::pair<anax::Entity, float>> list;
		int fullPercentCount = 0;

		//count resource amounts
		for(auto& r : res.map){
			ItemComponent& types = r.second.e.getComponent<ItemComponent>();
			for(Item::Type t : types.types){
				if(resType == t){
					StackableComponent& stack = r.second.e.getComponent<StackableComponent>();
					NameComponent& n = r.second.e.getComponent<NameComponent>();
					//res.count / consume amount = % satisfaction
					float p = stack.count / consumeValue;
					list.push_back(std::pair<anax::Entity, float>(r.second.e, p));
					resTotal += p;

					std::stringstream ss;
					ss << std::fixed << std::setprecision(2) << p;
					FileLogger::getInstance()->logString("\t" + n.name + "   %% satisfaction :" + ss.str() + "\n", "resourcesDebug");
					if(p >= 1.0f){
						++fullPercentCount;
					}
				}
			}
		}

		std::sort(list.begin(), list.end(), [](std::pair<anax::Entity, float>& left, std::pair<anax::Entity, float>& right) {
		    return left.second > right.second;
		});

		if(resTotal <= 1.0f){
			//resTotal <= 100% consumeValue : consume ALL resources
			for(auto& e : list){
				StackableComponent& stack = e.first.getComponent<StackableComponent>();
				stack.count = 0;
			}
		}else if(fullPercentCount > 0){
			distributeConsumption(consumeValue, list);

			//consume/reduce resource counts by their respective %'s
			for(auto& e : list){
				NameComponent& n = e.first.getComponent<NameComponent>();
				StackableComponent& stack = e.first.getComponent<StackableComponent>();

				std::stringstream ss;
				ss << std::fixed << std::setprecision(2) << stack.count;
				FileLogger::getInstance()->logString("\n\t###### " + n.name + " ; stack(QTY) : " + ss.str() + "\n", "resourcesDebug");

				stack.count -= e.second;

				ss.str("");
				ss << std::fixed << std::setprecision(2) << e.second;
				FileLogger::getInstance()->logString("\t\tADJUSTED RES CONSUMPTION : " + n.name + " ; consume resQty : " + ss.str() + "\n", "resourcesDebug");

				ss.str("");
				ss << std::fixed << std::setprecision(2) << stack.count;
				FileLogger::getInstance()->logString("\t$$$$$$ " + n.name + " ; stack(QTY) : " + ss.str() + "\n", "resourcesDebug");
			}

		}else{
			//sum satisfaction - 1.0f(100%) = % excess supply
			float pDiff = resTotal - 1.0f;

			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << pDiff;
			FileLogger::getInstance()->logString("\t\t%% excess supply : " + ss.str() + "\n", "resourcesDebug");

			for(auto& e : list){
				//% satisfaction - % excess supply / all res number = consume %
				float finalP = e.second - pDiff / list.size();

				//consume amount * consume % = resource qty to consume
				float resQty = consumeValue * finalP;
				StackableComponent& stack = e.first.getComponent<StackableComponent>();
				stack.count -= resQty;

				NameComponent& n = e.first.getComponent<NameComponent>();
				ss.str("");
				ss << std::fixed << std::setprecision(2) << finalP;
				std::string fPStr = ss.str();
				ss.str("");
				ss << std::fixed << std::setprecision(2) << resQty;
				FileLogger::getInstance()->logString("\t" + n.name + " ; consume %% : " + fPStr + " ; consume resQty : " + ss.str() + "\n\n", "resourcesDebug");
			}
		}
	}
	/*
	 * Adapted from code from https://www.mrexcel.com/forum/excel-questions/542441-how-divide-100%25-x-but-not-equal-increments-2.html
	 * How to divide 100% by "x", but not equal increments?
	 */
	void distributeConsumption(float consumeValue, std::vector<std::pair<anax::Entity, float>>& list){
		float ratioVar = 0.7f;

		float adjConCount = (consumeValue * (1.0f - ratioVar)) / (1.0f  - (std::pow(ratioVar, list.size())));
		list.front().second = adjConCount;

		for(int i = 1; i < list.size(); ++i){
			adjConCount = ratioVar * list[i - 1].second;

			list[i].second = adjConCount;
		}
	}
};

#endif /* LIBADO_ENTITY_SYSTEMS_TOWNRESOURCESYS_HPP_ */
