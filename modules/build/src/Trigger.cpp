#include <cstddef>

#include "Action.hpp"
#include "Trigger.hpp"
#include "BuildContext.hpp"

using std::list;

namespace redengine {
namespace build {

	Trigger::Trigger() {}

	struct ActionListUnref {

		list<Action*>* actions;

		ActionListUnref(list<Action*>* actions) : actions(actions) {}

		~ActionListUnref() {
			if(!actions)
				return;
			Trigger::ActionIterator begin(actions->begin()), end(actions->end());
			for(; begin != end; ++begin)
				(*begin)->unref();
		}

	};

	Trigger::Trigger(const Trigger& trigger) : ReferenceCounted(trigger) {
		ActionListUnref unref(&actions);
		ActionIterator begin(trigger.actions.begin()), end(trigger.actions.end());
		for(; begin != end; ++begin) {
			actions.push_back(*begin);
			(*begin)->ref();
		}
		unref.actions = NULL;
	}

	Trigger::~Trigger() {
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	void Trigger::addAction(Action* action) {
		if(!action)
			return;
		actions.push_back(action);
		action->ref();
	}

	bool Trigger::removeAction(Action* action) {
		if(!action)
			return false;
		list<Action*>::iterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin) {
			if(*begin == action) {
				actions.erase(begin);
				action->unref();
				return true;
			}
		}
		return false;
	}

	void Trigger::clearActions() {
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		actions.clear();
	}

	void Trigger::getActions(ActionIterator& begin, ActionIterator& end) const {
		begin = actions.begin();
		end = actions.end();
	}

	void Trigger::spin(BuildContext& context) {
		if(!isTriggered())
			return;
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			context.queueAction(*begin);
	}

	void Trigger::predictSpin(BuildContext& context) {
		if(!wouldTrigger())
			return;
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			context.queueAction(*begin);
	}

}}
