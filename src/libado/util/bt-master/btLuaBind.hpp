/*
 * btLuaBind.hpp
 *
 *  Created on: 21 Jul 2017
 *      Author: linbob
 */

#ifndef LIBADO_UTIL_BT_MASTER_BTLUABIND_HPP_
#define LIBADO_UTIL_BT_MASTER_BTLUABIND_HPP_

#include <memory>
#include <lua.hpp>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>

#include "bt.hpp"
#include "../../../gameScreen.hpp"

#include "../../entity/ai/regulatorDecotator.hpp"
#include "../../entity/ai/pathNearTgt.hpp"
#include "../../entity/ai/chkTgtDistance.hpp"
#include "../../entity/ai/hasPath.hpp"
#include "../../entity/ai/hasOwner.hpp"
#include "../../entity/ai/chkTgtMoved.hpp"
#include "../../entity/ai/debugDecorator.hpp"
#include "../../entity/ai/consumeEnergy.hpp"
#include "../../entity/ai/neverFail.hpp"
#include "../../entity/ai/hasMount.hpp"
#include "../../entity/ai/rideMount.hpp"
#include "../../entity/ai/turnLoop.hpp"
#include "../../entity/ai/consumeMoveEnergy.hpp"

namespace btLuaBind{

	static void addChild(bt::Node* parent, bt::Node::Ptr child){
		if(bt::Composite* c = dynamic_cast<bt::Composite*>(parent)){
			c->AddChild(child);
		}else if(bt::Decorator* d = dynamic_cast<bt::Decorator*>(parent)){
			d->SetChild(child);
		}
	}
	static void addChild(bt::BehaviorTree* tree, bt::Node* parent, bt::Node::Ptr child){
		if(!parent){
			tree->SetRoot(child);
		}else if(bt::Composite* c = dynamic_cast<bt::Composite*>(parent)){
			c->AddChild(child);
		}else if(bt::Decorator* d = dynamic_cast<bt::Decorator*>(parent)){
			d->SetChild(child);
		}
	}

	//C++ > Lua bindings for BT behavior tree system
	static void initBindings(sol::state& lua){

		lua.new_usertype<AiSystem>("AiSystem",
				"addTree", &AiSystem::addTree,
				"getTree", &AiSystem::getTree
		);

////////////////////////////////////////////////

		lua.new_usertype<bt::Node>("BtNode",
				"getBlackBoard", &bt::Node::GetSharedBlackboard,
				"getName", &bt::Node::getName,
				"setName" , &bt::Node::setName
		);
		lua.new_usertype<bt::BehaviorTree>("BtTree",
				"create", [](sol::this_state ts, std::string name) -> bt::Node*{
					sol::state_view lua(ts);
					std::shared_ptr<bt::BehaviorTree> tree = std::make_shared<bt::BehaviorTree>();

					((GameScreen*)lua["gameScreen"])->getAiSys()->addTree(tree, name);
					tree->setName(name);

					return tree.get();
				},
				"setRoot", &bt::BehaviorTree::SetRoot,
				sol::base_classes, sol::bases<bt::Node>()
		);

		lua.new_usertype<bt::Composite>("BtComposite",
				"addChild", &bt::Composite::AddChild
		);

		lua.new_usertype<bt::Selector>("BtSelector",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::Selector*{
					bt::Selector::Ptr s = std::make_shared<bt::Selector>(tree->GetSharedBlackboard());
					addChild(tree, parent, s);
					return s.get();
				},
				sol::base_classes, sol::bases<bt::Composite, bt::Node>()
		);
		lua.new_usertype<bt::Sequence>("BtSequence",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::Sequence*{
					bt::Sequence::Ptr s = std::make_shared<bt::Sequence>(tree->GetSharedBlackboard());
					addChild(tree, parent, s);
					return s.get();
				},
				sol::base_classes, sol::bases<bt::Composite, bt::Node>()
		);
		lua.new_usertype<bt::ParallelSequence>("BtParallelSequence",
				"createBool", [](bt::BehaviorTree* tree, bool succeedAll, bool failAll, bt::Node* parent = nullptr) -> bt::ParallelSequence*{
					bt::ParallelSequence::Ptr s = std::make_shared<bt::ParallelSequence>(tree->GetSharedBlackboard(), succeedAll, failAll);
					addChild(tree, parent, s);
					return s.get();
				},
				"createNum", [](bt::BehaviorTree* tree, int minSucceed, int minFail, bt::Node* parent = nullptr) -> bt::ParallelSequence*{
					bt::ParallelSequence::Ptr s = std::make_shared<bt::ParallelSequence>(tree->GetSharedBlackboard(), minSucceed, minFail);
					addChild(tree, parent, s);
					return s.get();
				},
				sol::base_classes, sol::bases<bt::Composite, bt::Node>()
		);

////////////////////////////////////////////////

		lua.new_usertype<bt::Decorator>("BtDecorator",
				"setChild", &bt::Decorator::SetChild,
				sol::base_classes, sol::bases<bt::Node>()
		);

		lua.new_usertype<bt::RandomDecorator>("BtRandom",
				"create", [](bt::BehaviorTree* tree, int f, bt::Node* parent = nullptr) -> bt::RandomDecorator*{
					std::shared_ptr<bt::RandomDecorator> r = std::make_shared<bt::RandomDecorator>(tree->GetSharedBlackboard(), f);
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::TaskCheckDecorator>("BtTaskCheck",
				"create", [](bt::BehaviorTree* tree, std::string s, bt::Node* parent = nullptr) -> bt::TaskCheckDecorator*{
					std::shared_ptr<bt::TaskCheckDecorator> r = std::make_shared<bt::TaskCheckDecorator>(s, tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::ResetInput>("BtResetInput",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::ResetInput*{
					std::shared_ptr<bt::ResetInput> r = std::make_shared<bt::ResetInput>(tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::TurnLoop>("BtTurnLoop",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::TurnLoop*{
					std::shared_ptr<bt::TurnLoop> r = std::make_shared<bt::TurnLoop>(tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::ChkTgtDistance>("BtChkTgtDistance",
				"create", [](sol::this_state ts, bool nearTgt , bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::ChkTgtDistance*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::ChkTgtDistance> r = std::make_shared<bt::ChkTgtDistance>(nearTgt, w, tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::HasOwner>("BtOwnerCheck",
				"create", [](bt::BehaviorTree* tree, bool f, bt::Node* parent = nullptr) -> bt::HasOwner*{
					std::shared_ptr<bt::HasOwner> r = std::make_shared<bt::HasOwner>(f, tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::TgtOwner>("BtTgtOwner",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::TgtOwner*{
					std::shared_ptr<bt::TgtOwner> r = std::make_shared<bt::TgtOwner>(tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::NeverFail>("BtNeverFail",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::NeverFail*{
					std::shared_ptr<bt::NeverFail> r = std::make_shared<bt::NeverFail>(tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::HasPath>("BtHasPath",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::HasPath*{
					std::shared_ptr<bt::HasPath> r = std::make_shared<bt::HasPath>(tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::HasMount>("BtHasMount",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::HasMount*{
					std::shared_ptr<bt::HasMount> r = std::make_shared<bt::HasMount>(tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::DebugDecorator>("BtDebugDecorator",
				"create", [](std::string i, std::string r, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::DebugDecorator*{
					std::shared_ptr<bt::DebugDecorator> d = std::make_shared<bt::DebugDecorator>(i, r, tree->GetSharedBlackboard());
					addChild(tree, parent, d);
					return d.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::Inverter>("BtInverter",
				"create", [](bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::Inverter*{
					std::shared_ptr<bt::Inverter> d = std::make_shared<bt::Inverter>(tree->GetSharedBlackboard());
					addChild(tree, parent, d);
					return d.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::ChkTgtMoved>("BtChkTgtMoved",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::ChkTgtMoved*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::ChkTgtMoved> r = std::make_shared<bt::ChkTgtMoved>(w, tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::ConsumeMoveEnergy>("BtConsumeMoveEnergy",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::ConsumeMoveEnergy*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::ConsumeMoveEnergy> r = std::make_shared<bt::ConsumeMoveEnergy>(w, tree->GetSharedBlackboard());
					addChild(tree, parent, r);
					return r.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

		lua.new_usertype<bt::ConsumeEnergy>("BtConsumeEnergy",
				"create", [](sol::this_state ts, int energyOverworld, int energyZone, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::ConsumeEnergy*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::ConsumeEnergy> d = std::make_shared<bt::ConsumeEnergy>(energyOverworld, energyZone, w, tree->GetSharedBlackboard());
					addChild(tree, parent, d);
					return d.get();
				},
				sol::base_classes, sol::bases<bt::Decorator, bt::Node>()
		);

////////////////////////////////////////////////

		lua.new_usertype<bt::Idle>("BtIdle",
				"create", [](bt::BehaviorTree* tree, int i, bt::Node* parent = nullptr) -> bt::Idle*{
					std::shared_ptr<bt::Idle> idle = std::make_shared<bt::Idle>(i, tree->GetSharedBlackboard());
					addChild(parent, idle);
					return idle.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
		lua.new_usertype<bt::PathToTgt>("BtPathToTgt",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::PathToTgt*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::PathToTgt> n = std::make_shared<bt::PathToTgt>(w, tree->GetSharedBlackboard());
					addChild(parent, n);
					return n.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
		lua.new_usertype<bt::PathNearTgt>("BtPathNearTgt",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::PathNearTgt*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::PathNearTgt> n = std::make_shared<bt::PathNearTgt>(w, tree->GetSharedBlackboard());
					addChild(parent, n);
					return n.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
		lua.new_usertype<bt::TgtRandomTile>("BtTgtRandomTile",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, int maxDist, bt::Node* parent = nullptr) -> bt::TgtRandomTile*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::TgtRandomTile> n = std::make_shared<bt::TgtRandomTile>(maxDist, w, tree->GetSharedBlackboard());
					addChild(parent, n);
					return n.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
		lua.new_usertype<bt::WalkToTgt>("BtWalkToTgt",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::WalkToTgt*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::WalkToTgt> n = std::make_shared<bt::WalkToTgt>(tree->GetSharedBlackboard(), w);
					addChild(parent, n);
					return n.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
		lua.new_usertype<bt::PickUpItem>("BtPickUpItem",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::PickUpItem*{
					sol::state_view lua(ts);
					tgui::Gui* g = ((GameScreen*)lua["gameScreen"])->getManager()->getGui();

					std::shared_ptr<bt::PickUpItem> n = std::make_shared<bt::PickUpItem>(g, tree->GetSharedBlackboard());
					addChild(parent, n);
					return n.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
		lua.new_usertype<bt::TradeShop>("BtTradeShop",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::TradeShop*{
					sol::state_view lua(ts);
					tgui::Gui* g = ((GameScreen*)lua["gameScreen"])->getManager()->getGui();

					std::shared_ptr<bt::TradeShop> n = std::make_shared<bt::TradeShop>(g, tree->GetSharedBlackboard());
					addChild(parent, n);
					return n.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
		lua.new_usertype<bt::RideMount>("BtRideMount",
				"create", [](sol::this_state ts, bt::BehaviorTree* tree, bt::Node* parent = nullptr) -> bt::RideMount*{
					sol::state_view lua(ts);
					Overworld* w = ((GameScreen*)lua["gameScreen"])->getOverworld();

					std::shared_ptr<bt::RideMount> n = std::make_shared<bt::RideMount>(tree->GetSharedBlackboard(), w);
					addChild(parent, n);
					return n.get();
				},
				sol::base_classes, sol::bases<bt::Leaf, bt::Node>()
		);
	}



}

#endif /* LIBADO_UTIL_BT_MASTER_BTLUABIND_HPP_ */
