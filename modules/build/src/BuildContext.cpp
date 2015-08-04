#include "Goal.hpp"
#include "BuildContext.hpp"

using std::string;
using redengine::io::DefaultConfiguredOutputStream;

namespace redengine {
namespace build {

	BuildContext::BuildContext(BuildUI& ui) : ui(ui), virtualTime(static_cast<time_t>(0u)) {}

	BuildContext::BuildContext(const BuildContext& context)
			: ui(context.ui), virtualTime(context.virtualTime), goals(context.goals) {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->ref();
	}

	BuildContext::~BuildContext() {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->unref();
	}

	time_t BuildContext::tickVirtualTime() {
		return ++virtualTime;
	}

	Goal* BuildContext::getGoal(const string& name) const {
		ConstGoalIterator it(goals.find(name));
		return it == goals.end() ? NULL : it->second;
	}

	bool BuildContext::addGoal(const string& name, Goal& goal) {
		if(goals.find(name) != goals.end())
			return false;
		goals[name] = &goal;
		goal.ref();
		return true;
	}

	bool BuildContext::removeGoal(const string& name) {
		GoalIterator it(goals.find(name));
		if(it == goals.end())
			return false;
		it->second->unref();
		return true;
	}

	void BuildContext::clearGoals() {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->unref();
		goals.clear();
	}

	void BuildContext::getGoals(GoalNameIterator& begin, GoalNameIterator& end) const {
		begin = GoalNameIterator(goals.begin());
		end = GoalNameIterator(goals.end());
	}

	void BuildContext::dumpContext(DefaultConfiguredOutputStream<char>::Stream&) const {
		//TODO
	}

}}
