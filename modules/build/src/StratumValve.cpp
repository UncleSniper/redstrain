#ifdef TESTING_REDSTRAIN_BUILD_API
#include <redstrain/io/streamoperators.hpp>
#endif /* TESTING_REDSTRAIN_BUILD_API */

#include "Action.hpp"
#include "StratumValve.hpp"
#include "BuildContext.hpp"

using std::set;
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

	StratumValve::StratumValve() {}

	StratumValve::StratumValve(const StratumValve& valve) : Valve(valve), actions(valve.actions) {
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	StratumValve::~StratumValve() {
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	bool StratumValve::addAction(Action* action) {
		if(!action)
			return false;
		if(!actions.insert(action).second)
			return false;
		action->ref();
		return true;
	}

	bool StratumValve::removeAction(Action* action) {
		set<Action*>::iterator it = actions.find(action);
		if(it == actions.end())
			return false;
		actions.erase(it);
		action->unref();
		return true;
	}

	void StratumValve::clearActions() {
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
		actions.clear();
	}

	void StratumValve::getActions(ActionIterator& begin, ActionIterator& end) const {
		begin = actions.begin();
		end = actions.end();
	}

	bool StratumValve::isOpen(BuildContext& context) {
		ActionIterator begin(actions.begin()), end(actions.end());
		for(; begin != end; ++begin) {
			if(!context.wasActionPerformed(*begin))
				return false;
		}
		return true;
	}

#ifdef TESTING_REDSTRAIN_BUILD_API
	void StratumValve::dumpValve(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "StratumValve " << this << " {" << endln << shift;
		stream << indent << "actions = {" << endln << shift;
		ActionIterator abegin(actions.begin()), aend(actions.end());
		for(; abegin != aend; ++abegin)
			(*abegin)->dumpAction(stream);
		stream << unshift << indent << '}' << endln;
		stream << unshift << indent << '}' << endln;
	}
#endif /* TESTING_REDSTRAIN_BUILD_API */

}}
