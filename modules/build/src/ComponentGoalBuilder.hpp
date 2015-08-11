#ifndef REDSTRAIN_MOD_BUILD_COMPONENTGOALBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTGOALBUILDER_HPP

#include <string>

#include "GoalBuilder.hpp"

namespace redengine {
namespace build {

	class Flavor;

	class REDSTRAIN_BUILD_API ComponentGoalBuilder : public GoalBuilder {

	  private:
		void setupNamedGoal(const Project&, BuildContext&, const std::string&) const;

	  protected:
		void setupFlavorGoal(const Project&, BuildContext&, const Flavor&) const;

	  public:
		ComponentGoalBuilder();
		ComponentGoalBuilder(const ComponentGoalBuilder&);

		virtual void setupGoals(const Project&, BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTGOALBUILDER_HPP */
