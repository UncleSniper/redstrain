#ifndef REDSTRAIN_MOD_BUILD_GOALBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_GOALBUILDER_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Project;
	class BuildContext;

	class REDSTRAIN_BUILD_API GoalBuilder {

	  public:
		GoalBuilder();
		GoalBuilder(const GoalBuilder&);
		virtual ~GoalBuilder();

		virtual void setupGoals(const Project&, BuildContext&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_GOALBUILDER_HPP */
