#include <redstrain/util/Unref.hpp>

#include "Action.hpp"
#include "Trigger.hpp"
#include "BuildContext.hpp"

using std::set;
using std::deque;
using redengine::util::Unref;

namespace redengine {
namespace build {

	BuildContext::BuildContext() {}

	BuildContext::BuildContext(const BuildContext& context)
			: triggers(context.triggers), actionQueue(context.actionQueue), actionSet(context.actionSet) {
		TriggerIterator tbegin(triggers.begin()), tend(triggers.end());
		for(; tbegin != tend; ++tbegin)
			(*tbegin)->ref();
		ActionQueueIterator abegin(actionQueue.begin()), aend(actionQueue.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->ref();
	}

	BuildContext::~BuildContext() {
		TriggerIterator tbegin(triggers.begin()), tend(triggers.end());
		for(; tbegin != tend; ++tbegin)
			(*tbegin)->unref();
		ActionQueueIterator abegin(actionQueue.begin()), aend(actionQueue.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->unref();
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
			action->perform(*this);
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
			action->wouldPerform(*this);
		}
		unrefQueue.actions = NULL;
	}

}}
