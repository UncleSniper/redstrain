#include <redstrain/io/streamoperators.hpp>

#include "MultiGoal.hpp"

using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::operator<<;

namespace redengine {
namespace build {

	MultiGoal::MultiGoal() {}

	MultiGoal::MultiGoal(const MultiGoal& goal) : Goal(goal), goals(goal.goals) {
		GoalIterator begin(goals.begin()), end(goals.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
	}

	MultiGoal::~MultiGoal() {
		GoalIterator begin(goals.begin()), end(goals.end());
		for(; begin != end; ++begin)
			(*begin)->unref();
	}

	bool MultiGoal::addGoal(Goal& goal) {
		if(!goals.append(&goal))
			return false;
		goal.ref();
		return true;
	}

	bool MultiGoal::removeGoal(Goal& goal) {
		if(!goals.erase(&goal))
			return false;
		goal.unref();
		return true;
	}

	void MultiGoal::clearGoals() {
		GoalIterator begin(goals.begin()), end(goals.end());
		for(; begin != end; ++begin)
			(*begin)->ref();
		goals.clear();
	}

	void MultiGoal::getGoals(GoalIterator& begin, GoalIterator& end) const {
		begin = goals.begin();
		end = goals.end();
	}

	void MultiGoal::attain(BuildContext& context) {
		GoalIterator begin(goals.begin()), end(goals.end());
		for(; begin != end; ++begin)
			(*begin)->attain(context);
	}

	void MultiGoal::wouldAttain(BuildContext& context) {
		GoalIterator begin(goals.begin()), end(goals.end());
		for(; begin != end; ++begin)
			(*begin)->wouldAttain(context);
	}

	void MultiGoal::dumpGoal(DefaultConfiguredOutputStream<char>::Stream& stream) const {
		stream << indent << "MultiGoal " << this << " {" << endln << shift;
		stream << indent << "goals = {" << endln << shift;
		GoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			(*gbegin)->dumpGoal(stream);
		stream << unshift << indent << '}' << endln;
		dumpGoalAspects(stream);
		stream << unshift << indent << '}' << endln;
	}

}}
