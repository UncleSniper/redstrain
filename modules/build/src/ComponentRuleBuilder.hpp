#ifndef REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTRULEBUILDER_HPP

#include "ProjectBuilder.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ComponentRuleBuilder : public ProjectBuilder::RuleBuilder {

	  private:
		Component::BuildDirectoryMapper& directoryMapper;
		Component::BuildArtifactMapper& artifactMapper;
		Component::ComponentTypeStringifier& typeStringifier;
		Component::ValveInjector* injector;

	  public:
		ComponentRuleBuilder(Component::BuildDirectoryMapper&, Component::BuildArtifactMapper&,
				Component::ComponentTypeStringifier&, Component::ValveInjector* = NULL);
		ComponentRuleBuilder(const ComponentRuleBuilder&);

		inline Component::BuildDirectoryMapper& getBuildDirectoryMapper() {
			return directoryMapper;
		}

		inline const Component::BuildDirectoryMapper& getBuildDirectoryMapper() const {
			return directoryMapper;
		}

		inline Component::BuildArtifactMapper& getBuildArtifactMapper() {
			return artifactMapper;
		}

		inline const Component::BuildArtifactMapper& getBuildArtifactMapper() const {
			return artifactMapper;
		}

		inline Component::ComponentTypeStringifier& getComponentTypeStringifier() {
			return typeStringifier;
		}

		inline const Component::ComponentTypeStringifier& getComponentTypeStringifier() const {
			return typeStringifier;
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
