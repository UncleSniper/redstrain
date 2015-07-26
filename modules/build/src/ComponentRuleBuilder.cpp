#include "ComponentRuleBuilder.hpp"

namespace redengine {
namespace build {

	ComponentRuleBuilder::ComponentRuleBuilder(Component::BuildDirectoryMapper& directoryMapper,
			Component::BuildArtifactMapper& artifactMapper, Component::ComponentTypeStringifier& typeStringifier,
			Component::ValveInjector* injector)
			: directoryMapper(directoryMapper), artifactMapper(artifactMapper), typeStringifier(typeStringifier),
			injector(injector) {}

	ComponentRuleBuilder::ComponentRuleBuilder(const ComponentRuleBuilder& builder)
			: RuleBuilder(builder), directoryMapper(builder.directoryMapper), artifactMapper(builder.artifactMapper),
			typeStringifier(builder.typeStringifier), injector(builder.injector) {}

	void ComponentRuleBuilder::setupRules(const Project&, Component& component, BuildContext& context) {
		component.setupRules(directoryMapper, artifactMapper, typeStringifier, context, injector);
	}

}}
