#ifndef REDSTRAIN_MOD_BUILD_PROJECTBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_PROJECTBUILDER_HPP

#include "Component.hpp"

namespace redengine {
namespace build {

	class Project;
	class BuildUI;
	class RuleBuilder;
	class BuildContext;
	class ProjectFactory;
	class ComponentFinder;
	class ComponentFactory;
	class DependencyResolver;

	class REDSTRAIN_BUILD_API ProjectBuilder {

	  private:
		Project* project;
		ProjectFactory& projectFactory;
		ComponentFinder& finder;
		ComponentFactory& componentFactory;
		DependencyResolver& resolver;
		RuleBuilder& ruleBuilder;

	  public:
		ProjectBuilder(ProjectFactory&, ComponentFinder&, ComponentFactory&, DependencyResolver&, RuleBuilder&);
		ProjectBuilder(const ProjectBuilder&);
		~ProjectBuilder();

		inline Project* getProject() const {
			return project;
		}

		inline ProjectFactory& getProjectFactory() {
			return projectFactory;
		}

		inline const ProjectFactory& getProjectFactory() const {
			return projectFactory;
		}

		inline ComponentFinder& getComponentFinder() {
			return finder;
		}

		inline const ComponentFinder& getComponentFinder() const {
			return finder;
		}

		inline ComponentFactory& getComponentFactory() {
			return componentFactory;
		}

		inline const ComponentFactory& getComponentFactory() const {
			return componentFactory;
		}

		inline DependencyResolver& getDependencyResolver() {
			return resolver;
		}

		inline const DependencyResolver& getDependencyResolver() const {
			return resolver;
		}

		inline RuleBuilder& getRuleBuilder() {
			return ruleBuilder;
		}

		inline const RuleBuilder& getRuleBuilder() const {
			return ruleBuilder;
		}

		void buildProject(const std::string&);
		Project* takeProject();
		BuildContext* newBuildContext(BuildUI&) const;

		void foundComponent(const std::string&, Component::Type);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_PROJECTBUILDER_HPP */
