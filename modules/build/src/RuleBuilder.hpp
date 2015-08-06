#ifndef REDSTRAIN_MOD_BUILD_RULEBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_RULEBUILDER_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Project;
	class Component;
	class BuildContext;

	class REDSTRAIN_BUILD_API RuleBuilder {

	  public:
		RuleBuilder();
		RuleBuilder(const RuleBuilder&);
		virtual ~RuleBuilder();

		virtual void setupRules(const Project&, Component&, BuildContext&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_RULEBUILDER_HPP */
