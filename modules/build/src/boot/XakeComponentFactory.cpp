#include <cctype>
#include <algorithm>
#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeComponentFactory.hpp"

using std::map;
using std::string;
using std::transform;
using redengine::util::Delete;
using redengine::util::Appender;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;

namespace redengine {
namespace build {
namespace boot {

	XakeComponentFactory::XakeComponentFactory(XakeProject& project) : project(project) {}

	XakeComponentFactory::XakeComponentFactory(const XakeComponentFactory& factory)
			: ComponentFactory(factory), project(factory.project) {}

	Component* XakeComponentFactory::newComponent(const Project&,
			const string& baseDirectory, Component::Type type) {
		Delete<XakeComponent> xcomponent(new XakeComponent(project, baseDirectory, type));
		string name(xcomponent->getComponentConfiguration().getProperty(Resources::RES_COMPONENT_NAME));
		if(name.empty())
			name = Pathname::basename(Pathname::tidy(baseDirectory));
		Delete<Component> component(new Component(type, name, baseDirectory));
		xcomponent->setComponent(*component);
		setupComponent(**component, **xcomponent);
		if(project.addComponent(*component, *xcomponent))
			xcomponent.set();
		return component.set();
	}

	static char slugify(char c) {
		if(
			(c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9')
		)
			return static_cast<char>(static_cast<unsigned char>(static_cast<unsigned>(toupper(
				static_cast<int>(static_cast<unsigned int>(static_cast<unsigned char>(c)))
			))));
		else
			return '_';
	}

	struct ExternalDependencyAppender : Appender<string> {

		Component& component;
		const Language& cpp;

		ExternalDependencyAppender(Component& component, const Language& cpp)
				: component(component), cpp(cpp) {}

		virtual void append(const string& dependency) {
			string name(StringUtils::trim(dependency));
			if(!name.empty())
				component.addExternalDependency(cpp, name);
		}

	};

	void XakeComponentFactory::setupComponent(Component& component, const XakeComponent& xcomponent) const {
		// set internal build name
		if(component.getType() != Component::EXECUTABLE) {
			string prjguard(project.getProjectConfiguration().getProperty(Resources::RES_PROJECT_GUARD));
			if(prjguard.empty()) {
				prjguard = project.getProjectName();
				transform(prjguard.begin(), prjguard.end(), prjguard.begin(), slugify);
			}
			string cmpguard(xcomponent.getComponentConfiguration().getProperty(Resources::RES_COMPONENT_GUARD));
			if(cmpguard.empty()) {
				cmpguard = component.getName();
				transform(cmpguard.begin(), cmpguard.end(), cmpguard.begin(), slugify);
			}
			string tpl(xcomponent.getComponentConfiguration().getProperty(Resources::RES_INTERNAL_API_MACRO));
			if(tpl.empty())
				tpl = project.getProjectConfiguration().getProperty(Resources::RES_INTERNAL_API_MACRO);
			map<string, string> variables;
			variables["project"] = prjguard;
			variables["module"] = cmpguard;
			component.setInternalBuildName(XakeUtils::subst(tpl, variables));
		}
		// add source directories
		component.addSourceDirectory(project.getProjectConfiguration().getProperty(Resources::RES_SOURCE_DIRECTORY));
		// add languages
		component.addLanguage(project.getCPPLanguage());
		component.addLanguage(project.getObjectFileLanguage());
		// add external dependencies
		const Language& cpp = *project.getCPPLanguage();
		ExternalDependencyAppender sink(component, cpp);
		string deps(xcomponent.getComponentConfiguration().getProperty(Resources::RES_EXTERNAL_LIBRARIES));
		if(!deps.empty())
			StringUtils::split(deps, ",", sink);
		Resources::ID osid;
		switch(project.getLinker()->getTargetOperatingSystem()) {
			case OS_LINUX:
				osid = Resources::RES_PFX_LINUX_EXTERNAL_LIBRARIES;
				break;
			case OS_WINDOWS:
				osid = Resources::RES_PFX_WINDOWS_EXTERNAL_LIBRARIES;
				break;
			default:
				osid = Resources::RES__LAST;
				break;
		}
		if(osid != Resources::RES__LAST) {
			deps = xcomponent.getComponentConfiguration().getProperty(osid);
			if(!deps.empty())
				StringUtils::split(deps, ",", sink);
		}
		deps = project.getProjectConfiguration().getProperty(Resources::RES_EXTERNAL_LIBRARIES);
		if(!deps.empty())
			StringUtils::split(deps, ",", sink);
		if(osid != Resources::RES__LAST) {
			deps = project.getProjectConfiguration().getProperty(osid);
			if(!deps.empty())
				StringUtils::split(deps, ",", sink);
		}
	}

}}}
