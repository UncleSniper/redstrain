#include <redstrain/util/Unref.hpp>

#include "Goal.hpp"
#include "BuildContext.hpp"
#include "FileArtifact.hpp"
#include "CyclicDependencyError.hpp"

using std::string;
using redengine::util::Unref;
using redengine::io::DefaultConfiguredOutputStream;

namespace redengine {
namespace build {

	BuildContext::BuildContext(BuildUI& ui) : ui(ui), virtualTime(static_cast<time_t>(0u)) {}

	BuildContext::BuildContext(const BuildContext& context)
			: ui(context.ui), virtualTime(context.virtualTime), goals(context.goals),
			fileArtifacts(context.fileArtifacts), buildingComponents(context.buildingComponents),
			builtComponents(context.builtComponents) {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->ref();
		FileArtifactIterator fabegin(fileArtifacts.begin()), faend(fileArtifacts.end());
		for(; fabegin != faend; ++fabegin)
			fabegin->second->ref();
	}

	BuildContext::~BuildContext() {
		ConstGoalIterator gbegin(goals.begin()), gend(goals.end());
		for(; gbegin != gend; ++gbegin)
			gbegin->second->unref();
		FileArtifactIterator fabegin(fileArtifacts.begin()), faend(fileArtifacts.end());
		for(; fabegin != faend; ++fabegin)
			fabegin->second->unref();
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

	FileArtifact& BuildContext::internFileArtifact(const string& path, const string& label) {
		FileArtifactIterator it = fileArtifacts.find(path);
		if(it != fileArtifacts.end())
			return *it->second;
		Unref<FileArtifact> file(new FileArtifact(path, label));
		fileArtifacts[path] = *file;
		return *file.set();
	}

	void BuildContext::beginBuildingComponent(Component& component) {
		if(buildingComponents.find(&component) != buildingComponents.end())
			throw CyclicDependencyError(component);
		buildingComponents.insert(&component);
	}

	void BuildContext::endBuildingComponent(const Component& component) {
		buildingComponents.erase(&component);
		builtComponents.insert(&component);
	}

	bool BuildContext::hasComponentBeenBuilt(const Component& component) const {
		return builtComponents.find(&component) != builtComponents.end();
	}

	void BuildContext::dumpContext(DefaultConfiguredOutputStream<char>::Stream&) const {
		//TODO
	}

}}
