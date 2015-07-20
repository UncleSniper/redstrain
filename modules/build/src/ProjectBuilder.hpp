#ifndef REDSTRAIN_MOD_BUILD_PROJECTBUILDER_HPP
#define REDSTRAIN_MOD_BUILD_PROJECTBUILDER_HPP

#include <string>

#include "Component.hpp"

namespace redengine {
namespace build {

	class Project;
	class BuildUI;

	class REDSTRAIN_BUILD_API ProjectBuilder {

	  public:
		class REDSTRAIN_BUILD_API ProjectFactory {

		  public:
			ProjectFactory();
			ProjectFactory(const ProjectFactory&);
			virtual ~ProjectFactory();

			virtual Project* newProject(const std::string&) = 0;

		};

		class REDSTRAIN_BUILD_API ComponentFinder {

		  public:
			ComponentFinder();
			ComponentFinder(const ComponentFinder&);
			virtual ~ComponentFinder();

			virtual void findComponents(const Project&, ProjectBuilder&) = 0;

		};

		class REDSTRAIN_BUILD_API ComponentFactory {

		  public:
			ComponentFactory();
			ComponentFactory(const ComponentFactory&);
			virtual ~ComponentFactory();

			virtual Component* newComponent(const Project&, const std::string&, Component::Type) = 0;

		};

		class REDSTRAIN_BUILD_API DependencyResolver {

		  public:
			DependencyResolver();
			DependencyResolver(const DependencyResolver&);
			virtual ~DependencyResolver();

			virtual void resolveDependencies(const Project&, Component&) = 0;

		};

		class REDSTRAIN_BUILD_API RuleBuilder {

		  public:
			RuleBuilder();
			RuleBuilder(const RuleBuilder&);
			virtual ~RuleBuilder();

			virtual void setupRules(const Project&, Component&, BuildContext&) = 0;

		};

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
