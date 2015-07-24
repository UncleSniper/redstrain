#include "ComponentRuleBuilder.hpp"

namespace redengine {
namespace build {

	ComponentRuleBuilder::ComponentRuleBuilder(Component::BuildDirectoryMapper& directoryMapper,
			Component::ComponentTypeStringifier& typeStringifier, Component::ValveInjector* injector)
			: directoryMapper(directoryMapper), typeStringifier(typeStringifier), injector(injector) {}

	ComponentRuleBuilder::ComponentRuleBuilder(const ComponentRuleBuilder& builder)
			: RuleBuilder(builder), directoryMapper(builder.directoryMapper),
			typeStringifier(builder.typeStringifier), injector(builder.injector) {}

	void ComponentRuleBuilder::setupRules(const Project&, Component& component, BuildContext& context) {
		component.setupRules(directoryMapper, typeStringifier, context, injector);
	}

}}
