#include <redstrain/util/Delete.hpp>
#include <redstrain/util/Unref.hpp>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Action.hpp"
#include "Trigger.hpp"
#include "BuildUI.hpp"
#include "ValveGroup.hpp"
#include "StaticValve.hpp"
#include "BuildContext.hpp"

using std::set;
using std::deque;
using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
#ifdef TESTING_REDSTRAIN_BUILD_API
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;
#endif /* TESTING_REDSTRAIN_BUILD_API */

namespace redengine {
namespace build {

	BuildContext::BuildContext(BuildUI& ui) : ui(ui) {}

	BuildContext::BuildContext(const BuildContext& context)
			: ui(context.ui), triggers(context.triggers), actionQueue(context.actionQueue),
			actionSet(context.actionSet), valves(context.valves), alreadyPerformed(context.alreadyPerformed),
			groups(context.groups) {
		TriggerIterator tbegin(triggers.begin()), tend(triggers.end());
		for(; tbegin != tend; ++tbegin)
			(*tbegin)->ref();
		ActionQueueIterator abegin(actionQueue.begin()), aend(actionQueue.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->ref();
		ConstValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin)
			vbegin->second->ref();
		ValveGroupIterator gbegin(groups.begin()), gend(groups.end());
		for(; gbegin != gend; ++gbegin)
			(*gbegin)->ref();
	}

	BuildContext::~BuildContext() {
		TriggerIterator tbegin(triggers.begin()), tend(triggers.end());
		for(; tbegin != tend; ++tbegin)
			(*tbegin)->unref();
		ActionQueueIterator abegin(actionQueue.begin()), aend(actionQueue.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->unref();
		ConstValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin)
			vbegin->second->unref();
		ValveGroupIterator gbegin(groups.begin()), gend(groups.end());
		for(; gbegin != gend; ++gbegin)
			(*gbegin)->unref();
	}

	bool BuildContext::addTrigger(Trigger* trigger) {
		if(!trigger)
			return false;
		if(triggers.find(trigger) != triggers.end())
			return false;
		triggers.insert(trigger);
		trigger->ref();
		return true;
	}

	bool BuildContext::removeTrigger(Trigger* trigger) {
		if(!trigger)
			return false;
		Triggers::iterator it(triggers.find(trigger));
		if(it == triggers.end())
			return false;
		triggers.erase(trigger);
		trigger->unref();
		return true;
	}

	void BuildContext::clearTriggers() {
		TriggerIterator begin(triggers.begin()), end(triggers.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		triggers.clear();
	}

	void BuildContext::getTriggers(TriggerIterator& begin, TriggerIterator& end) const {
		begin = triggers.begin();
		end = triggers.end();
	}

	StaticValve* BuildContext::getValve(const string& name) const {
		ConstValveIterator it = valves.find(name);
		return it == valves.end() ? NULL : it->second;
	}

	void BuildContext::addValve(const string& name, StaticValve* valve) {
		if(name.empty() || !valve)
			return;
		ConstValveIterator it = valves.find(name);
		if(it != valves.end()) {
			StaticValve* old = it->second;
			valves[name] = valve;
			valve->ref();
			old->unref();
		}
		else {
			valves[name] = valve;
			valve->ref();
		}
	}

	bool BuildContext::removeValve(const string& name) {
		ValveIterator it = valves.find(name);
		if(it == valves.end())
			return false;
		StaticValve* valve = it->second;
		valves.erase(it);
		valve->unref();
		return true;
	}

	void BuildContext::clearValves() {
		ConstValveIterator begin(valves.begin()), end(valves.end());
		for(; begin != end; ++begin)
			begin->second->unref();
		valves.clear();
	}

	StaticValve& BuildContext::getOrMakeValve(const string& name) {
		ConstValveIterator it = valves.find(name);
		if(it != valves.end())
			return *it->second;
		Delete<StaticValve> valve(new StaticValve);
		valves[name] = *valve;
		return *valve.set();
	}

	bool BuildContext::addValveGroup(ValveGroup* group) {
		if(!group)
			return false;
		if(!groups.insert(group).second)
			return false;
		group->ref();
		return true;
	}

	bool BuildContext::removeValveGroup(ValveGroup* group) {
		if(!groups.erase(group))
			return false;
		group->unref();
		return true;
	}

	void BuildContext::clearValveGroups() {
		ValveGroupIterator gbegin(groups.begin()), gend(groups.end());
		for(; gbegin != gend; ++gbegin)
			(*gbegin)->unref();
		groups.clear();
	}

	void BuildContext::getValveGroups(ValveGroupIterator& begin, ValveGroupIterator& end) const {
		begin = groups.begin();
		end = groups.end();
	}

	void BuildContext::forceValveGroups() {
		ValveGroupIterator gbegin(groups.begin()), gend(groups.end());
		for(; gbegin != gend; ++gbegin)
			(*gbegin)->forceDefaults(*this);
	}

	bool BuildContext::wasActionPerformed(Action* action) const {
		return alreadyPerformed.find(action) != alreadyPerformed.end();
	}

	void BuildContext::clearPerformedActions() {
		alreadyPerformed.clear();
	}

	void BuildContext::spinTriggers() {
		TriggerIterator begin(triggers.begin()), end(triggers.end());
		for(; begin != end; ++begin)
			(*begin)->spin(*this);
	}

	void BuildContext::predictTriggers() {
		TriggerIterator begin(triggers.begin()), end(triggers.end());
		for(; begin != end; ++begin)
			(*begin)->predictSpin(*this);
	}

	struct RemoveActionFromSet {

		set<Action*>* actions;

		Action* action;

		RemoveActionFromSet(set<Action*>* actions, Action* action) : actions(actions), action(action) {}

		~RemoveActionFromSet() {
			if(actions && action)
				actions->erase(action);
		}

	};

	bool BuildContext::queueAction(Action* action) {
		if(!action)
			return false;
		if(actionSet.find(action) != actionSet.end())
			return false;
		actionSet.insert(action);
		RemoveActionFromSet remove(&actionSet, action);
		actionQueue.push_back(action);
		remove.actions = NULL;
		action->ref();
		return true;
	}

	struct UnrefActionQueue {

		deque<Action*>* actions;

		UnrefActionQueue(deque<Action*>* actions) : actions(actions) {}

		~UnrefActionQueue() {
			if(!actions)
				return;
			deque<Action*>::const_iterator begin(actions->begin()), end(actions->end());
			for(; begin != end; ++begin)
				(*begin)->unref();
		}

	};

	void BuildContext::performActions() {
		ActionQueue buffer(actionQueue);
		actionQueue.clear();
		UnrefActionQueue unrefQueue(&buffer);
		while(!buffer.empty()) {
			Unref<Action> action(buffer.front());
			buffer.pop_front();
			ui.willPerformAction(**action);
			action->perform(*this);
			alreadyPerformed.insert(*action);
		}
		unrefQueue.actions = NULL;
	}

	void BuildContext::predictActions() {
		ActionQueue buffer(actionQueue);
		actionQueue.clear();
		UnrefActionQueue unrefQueue(&buffer);
		while(!buffer.empty()) {
			Unref<Action> action(buffer.front());
			buffer.pop_front();
			ui.wouldPerformAction(**action);
			action->wouldPerform(*this);
			alreadyPerformed.insert(*action);
		}
		unrefQueue.actions = NULL;
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void BuildContext::dumpContext(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "BuildContext {" << endln << shift;
		// triggers
		stream << indent << "triggers = {" << endln << shift;
		TriggerIterator tbegin(triggers.begin()), tend(triggers.end());
		for(; tbegin != tend; ++tbegin)
			(*tbegin)->dumpTrigger(stream);
		stream << unshift << indent << '}' << endln;
		// valves
		stream << indent << "valves = {" << endln << shift;
		ConstValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin) {
			stream << indent << vbegin->first << " -> " << endln << shift;
			vbegin->second->dumpValve(stream);
			stream << unshift;
		}
		stream << unshift << indent << '}' << endln;
		// valve groups
		stream << indent << "valveGroups = {" << endln << shift;
		ValveGroupIterator vgbegin(groups.begin()), vgend(groups.end());
		for(; vgbegin != vgend; ++vgbegin)
			(*vgbegin)->dumpValveGroup(stream);
		stream << unshift << indent << '}' << endln;
		// done
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
