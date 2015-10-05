#ifndef REDSTRAIN_MOD_BUILD_GOALPROPERTYINJECTOR_HPP
#define REDSTRAIN_MOD_BUILD_GOALPROPERTYINJECTOR_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Goal;
	class Component;

	class REDSTRAIN_BUILD_API GoalPropertyInjector {

	  public:
		GoalPropertyInjector();
		GoalPropertyInjector(const GoalPropertyInjector&);
		virtual ~GoalPropertyInjector();

		virtual void injectGoalProperties(Component&, Goal&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GOALPROPERTYINJECTOR_HPP */
