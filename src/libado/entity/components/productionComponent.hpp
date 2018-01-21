/*
 * productionComponent.hpp
 *
 *  Created on: 2 Jan 2017
 *      Author: linbob
 */

#ifndef LIBADO_ENTITY_COMPONENTS_PRODUCTIONCOMPONENT_HPP_
#define LIBADO_ENTITY_COMPONENTS_PRODUCTIONCOMPONENT_HPP_

#include <anax/Component.hpp>

struct ProductionComponent : anax::Component{
	//resource name, pair<pair<productionQty, lastQtyProduced>, workerDistribution>
	std::map<std::string, std::pair<std::pair<float, float>, float>> producedResources;
	const float baseProductionRate = 2.8f;

	ProductionComponent(){}

};

#endif /* LIBADO_ENTITY_COMPONENTS_PRODUCTIONCOMPONENT_HPP_ */
