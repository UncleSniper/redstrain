#include <cstdlib>
#include <iostream>
#include <redstrain/util/Unref.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/utils.hpp>

#include "Project.hpp"
#include "RuleBuilder.hpp"
#include "GoalBuilder.hpp"
#include "BuildContext.hpp"
#include "ProjectBuilder.hpp"
#include "ProjectFactory.hpp"
#include "ComponentFinder.hpp"
#include "ComponentFactory.hpp"
#include "DependencyResolver.hpp"
#include "DuplicateComponentError.hpp"

using std::cerr;
using std::endl;
using std::string;
using redengine::util::Unref;
using redengine::util::Delete;
using redengine::platform::currentTimeMillis;

namespace redengine {
namespace build {

	ProjectBuilder::ProjectBuilder(ProjectFactory& projectFactory, ComponentFinder& finder,
			ComponentFactory& componentFactory, DependencyResolver& resolver, RuleBuilder& ruleBuilder,
			GoalBuilder& goalBuilder) : project(NULL), projectFactory(projectFactory), finder(finder),
			componentFactory(componentFactory), resolver(resolver), ruleBuilder(ruleBuilder),
			goalBuilder(goalBuilder) {}

	ProjectBuilder::ProjectBuilder(const ProjectBuilder& builder) : project(NULL),
			projectFactory(builder.projectFactory), finder(builder.finder),
			componentFactory(builder.componentFactory), resolver(builder.resolver),
			ruleBuilder(builder.ruleBuilder), goalBuilder(builder.goalBuilder) {}

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
		uint64_t start = currentTimeMillis();
		finder.findComponents(*project, *this);
		Project::ComponentNameIterator cnbegin, cnend;
		project->getComponents(cnbegin, cnend);
		for(; cnbegin != cnend; ++cnbegin)
			resolver.resolveDependencies(*project, *project->getComponent(*cnbegin));
		uint64_t delta = currentTimeMillis() - start;
		if(getenv("REDBUILD_TIME_PROJECT_BUILDER"))
			cerr << "***DEBUG: Built project model in " << delta << " msec" << endl;
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
			throw DuplicateComponentError(component->getName());
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
		goalBuilder.setupGoals(*project, **context);
		return context.set();
	}

}}
