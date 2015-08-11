#include <redstrain/util/Unref.hpp>

#include "Project.hpp"
#include "Component.hpp"
#include "MultiGoal.hpp"
#include "BuildContext.hpp"
#include "ComponentGoalBuilder.hpp"

using std::string;
using redengine::util::Unref;

namespace redengine {
namespace build {

	ComponentGoalBuilder::ComponentGoalBuilder() {}

	ComponentGoalBuilder::ComponentGoalBuilder(const ComponentGoalBuilder& builder) : GoalBuilder(builder) {}

	void ComponentGoalBuilder::setupNamedGoal(const Project& project, BuildContext& context,
			const string& name) const {
		Unref<MultiGoal> groupGoal(new MultiGoal);
		Project::ComponentNameIterator cnbegin, cnend;
		project.getComponents(cnbegin, cnend);
		for(; cnbegin != cnend; ++cnbegin) {
			Goal* goal = context.getGoal(project.getComponent(*cnbegin)->getGoalName() + ':' + name);
			if(goal)
				groupGoal->addGoal(*goal);
		}
		context.addGoal(name, **groupGoal);
	}

	void ComponentGoalBuilder::setupFlavorGoal(const Project& project, BuildContext& context,
			const Flavor& flavor) const {
		setupNamedGoal(project, context, flavor.getName());
	}

	void ComponentGoalBuilder::setupGoals(const Project& project, BuildContext& context) {
		setupNamedGoal(project, context, "build");
		setupNamedGoal(project, context, "clean");
		Goal* defaultGoal = context.getGoal("build");
		if(defaultGoal)
			context.setDefaultGoal(defaultGoal);
	}

}}
