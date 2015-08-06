#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/error/IllegalArgumentError.hpp>

#include "Project.hpp"
#include "RuleBuilder.hpp"
#include "BuildContext.hpp"
#include "ProjectBuilder.hpp"
#include "ProjectFactory.hpp"
#include "ComponentFinder.hpp"
#include "ComponentFactory.hpp"
#include "DependencyResolver.hpp"

using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::error::IllegalArgumentError;

namespace redengine {
namespace build {

	ProjectBuilder::ProjectBuilder(ProjectFactory& projectFactory, ComponentFinder& finder,
			ComponentFactory& componentFactory, DependencyResolver& resolver, RuleBuilder& ruleBuilder)
			: project(NULL), projectFactory(projectFactory), finder(finder), componentFactory(componentFactory),
			resolver(resolver), ruleBuilder(ruleBuilder) {}

	ProjectBuilder::ProjectBuilder(const ProjectBuilder& builder) : project(NULL),
			projectFactory(builder.projectFactory), finder(builder.finder),
			componentFactory(builder.componentFactory), resolver(builder.resolver), ruleBuilder(ruleBuilder) {}

	ProjectBuilder::~ProjectBuilder() {
		if(project)
			delete project;
	}

	void ProjectBuilder::buildProject(const string& baseDirectory) {
		if(project) {
			delete project;
			project = NULL;
		}
		project = projectFactory.newProject(baseDirectory);
		if(!project)
			return;
		finder.findComponents(*project, *this);
		Project::ComponentNameIterator cnbegin, cnend;
		project->getComponents(cnbegin, cnend);
		for(; cnbegin != cnend; ++cnbegin)
			resolver.resolveDependencies(*project, *project->getComponent(*cnbegin));
	}

	Project* ProjectBuilder::takeProject() {
		Project* p = project;
		project = NULL;
		return p;
	}

	void ProjectBuilder::foundComponent(const string& baseDirectory, Component::Type type) {
		if(!project)
			return;
		Unref<Component> component(componentFactory.newComponent(*project, baseDirectory, type));
		if(!*component)
			return;
		if(project->getComponent(component->getType() == Component::EXECUTABLE
				? Project::EXECUTABLE : Project::LIBRARY, component->getName()))
			throw IllegalArgumentError("A component named '" + component->getName()
					+ "' already exists in the project");
		project->addComponent(**component);
	}

	BuildContext* ProjectBuilder::newBuildContext(BuildUI& ui) const {
		if(!project)
			return NULL;
		Delete<BuildContext> context(new BuildContext(ui));
		Project::ComponentNameIterator cnbegin, cnend;
		project->getComponents(cnbegin, cnend);
		for(; cnbegin != cnend; ++cnbegin)
			ruleBuilder.setupRules(*project, *project->getComponent(*cnbegin), **context);
		return context.set();
	}

}}
