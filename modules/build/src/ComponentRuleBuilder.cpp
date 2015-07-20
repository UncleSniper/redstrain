#include "ComponentRuleBuilder.hpp"

namespace redengine {
namespace build {

	ComponentRuleBuilder::ComponentRuleBuilder(Component::BuildDirectoryMapper& directoryMapper,
			Component::ValveInjector* injector) : directoryMapper(directoryMapper), injector(injector) {}

	ComponentRuleBuilder::ComponentRuleBuilder(const ComponentRuleBuilder& builder)
			: RuleBuilder(builder), directoryMapper(builder.directoryMapper), injector(builder.injector) {}

	void ComponentRuleBuilder::setupRules(const Project&, Component& component, BuildContext& context) {
		component.setupRules(directoryMapper, context, injector);
	}

}}
