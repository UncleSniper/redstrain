#include "Goal.hpp"
#include "DefaultGoalPropertyInjector.hpp"
#include "DefaultComponentTypeStringifier.hpp"

namespace redengine {
namespace build {

	DefaultGoalPropertyInjector::DefaultGoalPropertyInjector(ComponentTypeStringifier* typeStringifier)
			: typeStringifier(typeStringifier) {}

	DefaultGoalPropertyInjector::DefaultGoalPropertyInjector(const DefaultGoalPropertyInjector& injector)
			: GoalPropertyInjector(injector), typeStringifier(injector.typeStringifier) {}

	void DefaultGoalPropertyInjector::injectGoalProperties(Component& component, Goal& goal) {
		if(typeStringifier)
			goal.setComponentType(typeStringifier->stringifyComponentType(component.getType()));
		else {
			switch(component.getType()) {
				#define clamp(c) \
					case Component::c: \
						goal.setComponentType(DefaultComponentTypeStringifier::DEFAULT_ ## c ## _TAG); \
						break;
				clamp(LIBRARY)
				clamp(EXECUTABLE)
				clamp(DATA)
				clamp(BLOB)
				#undef clamp
				default:
					break;
			}
		}
		goal.setComponentName(component.getName());
		goal.setComponentBaseDirectory(component.getBaseDirectory());
	}

}}
