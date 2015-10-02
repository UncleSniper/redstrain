#include <redstrain/util/StringUtils.hpp>

#include "../Project.hpp"
#include "XakeComponent.hpp"
#include "XakeDependencyResolver.hpp"
#include "../UnresolvedComponentDependencyError.hpp"

using std::string;
using redengine::util::Appender;
using redengine::util::StringUtils;

namespace redengine {
namespace build {
namespace boot {

	XakeDependencyResolver::XakeDependencyResolver() {}

	XakeDependencyResolver::XakeDependencyResolver(const XakeDependencyResolver& resolver)
			: DependencyResolver(resolver) {}

	struct DependencyAppender : Appender<string> {

		const Project& project;
		Component& component;

		DependencyAppender(const Project& project, Component& component)
				: project(project), component(component) {}

		virtual void append(const string& dependency) {
			string name(StringUtils::trim(dependency));
			if(name.empty())
				return;
			Component* dc = project.getComponent(Project::LIBRARY, name);
			if(!dc)
				throw UnresolvedComponentDependencyError(component.getName(), name);
			component.addDependency(*dc);
		}

	};

	void XakeDependencyResolver::resolveDependencies(const Project& project, Component& component) {
		const XakeComponent* xcomponent = dynamic_cast<const XakeComponent*>(&component);
		if(!xcomponent)
			return;
		string deps(xcomponent->getComponentConfiguration().getProperty(Resources::RES_DEPEND_MODULES));
		if(deps.empty())
			return;
		DependencyAppender sink(project, component);
		StringUtils::split(deps, ",", sink);
	}

}}}
