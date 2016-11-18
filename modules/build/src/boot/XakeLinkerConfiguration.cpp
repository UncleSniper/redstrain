#include <redstrain/util/StringUtils.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeLinkerConfiguration.hpp"
#include "XakeBuildArtifactMapper.hpp"
#include "../platform.hpp"

using std::list;
using std::string;
using redengine::util::Appender;
using redengine::util::StringUtils;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;

namespace redengine {
namespace build {
namespace boot {

	XakeLinkerConfiguration::XakeLinkerConfiguration(XakeComponent& component, bool forStatic)
			: component(component), forStatic(forStatic) {}

	XakeLinkerConfiguration::XakeLinkerConfiguration(const XakeLinkerConfiguration& configuration)
			: LinkerConfiguration(configuration), component(configuration.component),
			forStatic(configuration.forStatic) {}

	struct ExternalDependencyAppender : Appender<string> {

		Linkage& linkage;

		ExternalDependencyAppender(Linkage& linkage) : linkage(linkage) {}

		virtual void append(const string& dependency) {
			string name(StringUtils::trim(dependency));
			if(!name.empty())
				linkage.addLibrary(dependency);
		}

	};

	void XakeLinkerConfiguration::applyConfiguration(Linkage& linkage) {
		XakeProject& project = component.getProject();
		const Language& objlang = project.getObjectFileLanguage();
		// dependencies
		if(component.getType() == Component::EXECUTABLE
				|| requiresInterlinkedLibraries(project.getLinker().getTargetOperatingSystem())) {
			list<Component*> dependencies;
			component.getTransitiveDependencies(dependencies);
			XakeBuildArtifactMapper artifactMapper(project);
			list<Component*>::const_iterator cdbegin(dependencies.begin()), cdend(dependencies.end());
			for(; cdbegin != cdend; ++cdbegin) {
				linkage.addLibraryDirectory((*cdbegin)->getBaseDirectory());
				linkage.addLibrary(artifactMapper.getTargetFileName(**cdbegin, objlang,
						forStatic ? Flavor::STATIC : Flavor::DYNAMIC));
			}
		}
		// external dependencies
		ExternalDependencyAppender sink(linkage);
		string deps(component.getComponentConfiguration().getProperty(Resources::RES_EXTERNAL_LIBRARIES));
		if(!deps.empty())
			StringUtils::split(deps, ",", sink);
		Resources::ID osid;
		switch(project.getLinker().getTargetOperatingSystem()) {
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
			deps = component.getComponentConfiguration().getProperty(osid);
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
		Resources::ID lmid;
		switch(linkage.getLinkMode()) {
			case Linkage::STATIC_EXECUTABLE:
			case Linkage::STATIC_LIBRARY:
				lmid = Resources::RES_PFX_STATIC_EXTERNAL_LIBRARIES;
				break;
			case Linkage::DYNAMIC_EXECUTABLE:
			case Linkage::DYNAMIC_LIBRARY:
				lmid = Resources::RES_PFX_DYNAMIC_EXTERNAL_LIBRARIES;
				break;
			default:
				lmid = Resources::RES__LAST;
				break;
		}
		if(lmid != Resources::RES__LAST) {
			deps = component.getComponentConfiguration().getProperty(lmid);
			if(!deps.empty())
				StringUtils::split(deps, ",", sink);
			deps = project.getProjectConfiguration().getProperty(lmid);
			if(!deps.empty())
				StringUtils::split(deps, ",", sink);
		}
	}

}}}
