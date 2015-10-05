#ifndef REDSTRAIN_MOD_BUILD_DEFAULTGOALPROPERTYINJECTOR_HPP
#define REDSTRAIN_MOD_BUILD_DEFAULTGOALPROPERTYINJECTOR_HPP

#include "GoalPropertyInjector.hpp"

namespace redengine {
namespace build {

	class ComponentTypeStringifier;

	class REDSTRAIN_BUILD_API DefaultGoalPropertyInjector : public GoalPropertyInjector {

	  private:
		ComponentTypeStringifier* typeStringifier;

	  public:
		DefaultGoalPropertyInjector(ComponentTypeStringifier* = NULL);
		DefaultGoalPropertyInjector(const DefaultGoalPropertyInjector&);

		inline ComponentTypeStringifier* getComponentTypeStringifier() const {
			return typeStringifier;
		}

		inline void setComponentTypeStringifier(ComponentTypeStringifier* typeStringifier) {
			this->typeStringifier = typeStringifier;
		}

		virtual void injectGoalProperties(Component&, Goal&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DEFAULTGOALPROPERTYINJECTOR_HPP */
