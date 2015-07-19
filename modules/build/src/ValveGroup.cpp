#include "ValveGroup.hpp"
#include "StaticValve.hpp"

namespace redengine {
namespace build {

	ValveGroup::ValveGroup() {}

	ValveGroup::ValveGroup(const ValveGroup& group)
			: ReferenceCounted(group), valves(group.valves), defaults(group.defaults) {
		ValveIterator mbegin(valves.begin()), mend(valves.end());
		for(; mbegin != mend; ++mbegin)
			(*mbegin)->ref();
		StaticValveIterator dbegin(defaults.begin()), dend(defaults.end());
		for(; dbegin != dend; ++dbegin)
			(*dbegin)->ref();
	}

	ValveGroup::~ValveGroup() {
		ValveIterator mbegin(valves.begin()), mend(valves.end());
		for(; mbegin != mend; ++mbegin)
			(*mbegin)->unref();
		StaticValveIterator dbegin(defaults.begin()), dend(defaults.end());
		for(; dbegin != dend; ++dbegin)
			(*dbegin)->unref();
	}

	bool ValveGroup::addMember(Valve* valve) {
		if(!valve)
			return false;
		if(!valves.insert(valve).second)
			return false;
		valve->ref();
		return true;
	}

	bool ValveGroup::removeMember(Valve* valve) {
		if(!valves.erase(valve))
			return false;
		valve->unref();
		return true;
	}

	void ValveGroup::clearMembers() {
		ValveIterator begin(valves.begin()), end(valves.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	void ValveGroup::getMembers(ValveIterator& begin, ValveIterator& end) const {
		begin = valves.begin();
		end = valves.end();
	}

	bool ValveGroup::addDefault(StaticValve* valve) {
		if(!valve)
			return false;
		if(!defaults.insert(valve).second)
			return false;
		valve->ref();
		return true;
	}

	bool ValveGroup::removeDefault(StaticValve* valve) {
		if(!defaults.erase(valve))
			return false;
		valve->unref();
		return true;
	}

	void ValveGroup::clearDefaults() {
		StaticValveIterator begin(defaults.begin()), end(defaults.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	void ValveGroup::getDefaults(StaticValveIterator& begin, StaticValveIterator& end) const {
		begin = defaults.begin();
		end = defaults.end();
	}

	bool ValveGroup::forceDefaults(BuildContext& context) {
		ValveIterator mbegin(valves.begin()), mend(valves.end());
		for(; mbegin != mend; ++mbegin) {
			if(!(*mbegin)->isOpen(context))
				return false;
		}
		StaticValveIterator dbegin(defaults.begin()), dend(defaults.end());
		for(; dbegin != dend; ++dbegin)
			(*dbegin)->setOpen(true);
		return true;
	}

}}