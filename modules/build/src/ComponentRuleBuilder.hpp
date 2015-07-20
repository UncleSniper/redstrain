#ifndef REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP

#include "ProjectBuilder.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ComponentRuleBuilder : public ProjectBuilder::RuleBuilder {

	  private:
		Component::BuildDirectoryMapper& directoryMapper;
		Component::ValveInjector* injector;

	  public:
		ComponentRuleBuilder(Component::BuildDirectoryMapper&, Component::ValveInjector* = NULL);
		ComponentRuleBuilder(const ComponentRuleBuilder&);

		inline Component::BuildDirectoryMapper& getBuildDirectoryMapper() {
			return directoryMapper;
		}

		inline const Component::BuildDirectoryMapper& getBuildDirectoryMapper() const {
			return directoryMapper;
		}

		inline Component::ValveInjector* getValveInjector() const {
			return injector;
		}

		inline void setValveInjector(Component::ValveInjector* injector = NULL) {
			this->injector = injector;
		}

		virtual void setupRules(const Project&, Component&, BuildContext&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP */
