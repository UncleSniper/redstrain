#include <cstddef>
#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Valve.hpp"
#include "Action.hpp"
#include "Trigger.hpp"
#include "BuildContext.hpp"

using std::set;
using std::list;
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

	Trigger::Trigger() {}

	Trigger::Trigger(const Trigger& trigger)
			: ReferenceCounted(trigger), actions(trigger.actions), valves(trigger.valves) {
		ActionIterator abegin(actions.begin()), end(actions.end());
		for(; abegin != end; ++abegin)
			(*abegin)->ref();
		ValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin)
			(*vbegin)->ref();
	}

	Trigger::~Trigger() {
		ActionIterator abegin(actions.begin()), aend(actions.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->unref();
		ValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin)
			(*vbegin)->unref();
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

	void Trigger::addValve(Valve* valve) {
		if(!valve)
			return;
		valves.insert(valve);
		valve->ref();
	}

	bool Trigger::removeValve(Valve* valve) {
		if(!valve)
			return false;
		if(!valves.erase(valve))
			return false;
		valve->unref();
		return true;
	}

	void Trigger::clearValves() {
		ValveIterator begin(valves.begin()), end(valves.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		valves.clear();
	}

	void Trigger::getValves(ValveIterator& begin, ValveIterator& end) const {
		begin = valves.begin();
		end = valves.end();
	}

	bool Trigger::areValvesOpen(BuildContext& context) const {
		ValveIterator begin(valves.begin()), end(valves.end());
		for(; begin != end; ++begin) {
			if(!(*begin)->isOpen(context))
				return false;
		}
		return true;
	}

	void Trigger::spin(BuildContext& context) {
		if(!areValvesOpen(context) || !isTriggered(context))
			return;
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			context.queueAction(*begin);
	}

	void Trigger::predictSpin(BuildContext& context) {
		if(!areValvesOpen(context) || !wouldTrigger(context))
			return;
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			context.queueAction(*begin);
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void Trigger::dumpTriggerAspects(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		// actions
		stream << indent << "actions = {" << endln << shift;
		ActionIterator abegin(actions.begin()), aend(actions.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpAction(stream);
		stream << unshift << indent << '}' << endln;
		// valves
		stream << indent << "valves = {" << endln << shift;
		ValveIterator vbegin(valves.begin()), vend(valves.end());
		for(; vbegin != vend; ++vbegin)
			(*vbegin)->dumpValve(stream);
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
