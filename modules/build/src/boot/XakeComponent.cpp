#include <set>
#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/redmond/constants.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeBuildArtifactMapper.hpp"
#include "../platform.hpp"

using std::set;
using std::list;
using std::string;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::redmond::buildTargetOS;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;

namespace redengine {
namespace build {
namespace boot {

	// ======== XakeLinkerConfiguration ========

	XakeComponent::XakeLinkerConfiguration::XakeLinkerConfiguration(XakeComponent& component, bool forStatic)
			: component(component), forStatic(forStatic) {}

	XakeComponent::XakeLinkerConfiguration::XakeLinkerConfiguration(const XakeLinkerConfiguration& configuration)
			: LinkerConfiguration(configuration), component(configuration.component),
			forStatic(configuration.forStatic) {}

	void XakeComponent::XakeLinkerConfiguration::applyConfiguration(Linkage& linkage) {
		Component* c = component.getComponent();
		if(c) {
			XakeProject& project = component.getProject();
			const Language& objlang = *project.getObjectFileLanguage();
			// dependencies
			if(c->getType() == Component::EXECUTABLE
					|| requiresInterlinkedLibraries(project.getLinker()->getTargetOperatingSystem())) {
				list<Component*> dependencies;
				c->getTransitiveDependencies(dependencies);
				XakeBuildArtifactMapper artifactMapper(project);
				list<Component*>::const_iterator cdbegin(dependencies.begin()), cdend(dependencies.end());
				for(; cdbegin != cdend; ++cdbegin) {
					linkage.addLibraryDirectory((*cdbegin)->getBaseDirectory());
					linkage.addLibrary(artifactMapper.getTargetFileName(**cdbegin, objlang,
							forStatic ? Flavor::STATIC : Flavor::DYNAMIC));
				}
			}
			// external dependencies
			Component::DependencyIterator edbegin, edend;
			c->getExternalDependencies(*project.getCPPLanguage(), edbegin, edend);
			for(; edbegin != edend; ++edbegin)
				linkage.addLibrary(*edbegin);
		}
	}

	// ======== XakeComponent ========

	const char *const XakeComponent::DEFAULT_COMMON_MODULES_PROPERTIES_FILE = "modules.properties";
	const char *const XakeComponent::DEFAULT_COMMON_TOOLS_PROPERTIES_FILE = "tools.properties";
	const char *const XakeComponent::DEFAULT_COMPONENT_PROPERTIES_FILE = "component.properties";

	XakeComponent::XakeComponent(XakeProject& project, const string& baseDirectory, Component::Type type)
			: project(project), baseDirectory(baseDirectory), type(type), component(NULL),
			staticLinkerConfiguration(*this, true), dynamicLinkerConfiguration(*this, false) {
		Resources::ID typeres;
		const char* typedefault;
		switch(type) {
			case Component::LIBRARY:
				typeres = Resources::RES_COMMON_MODULES_PROPERTIES_FILE;
				typedefault = XakeComponent::DEFAULT_COMMON_MODULES_PROPERTIES_FILE;
				break;
			case Component::EXECUTABLE:
				typeres = Resources::RES_COMMON_TOOLS_PROPERTIES_FILE;
				typedefault = XakeComponent::DEFAULT_COMMON_TOOLS_PROPERTIES_FILE;
				break;
			default:
				typeres = Resources::RES__LAST;
				typedefault = NULL;
				break;
		}
		const Resources& prjconf = project.getProjectConfiguration();
		Stat stat;
		if(typedefault) {
			string typedir(Pathname::join(baseDirectory, Pathname::PARENT_DIRECTORY));
			string typebase(prjconf.getProperty(typeres));
			if(typebase.empty())
				typebase = typedefault;
			string typepath(Pathname::join(typedir, typebase));
			if(Filesystem::access(typepath, Filesystem::FILE_EXISTS)) {
				Filesystem::stat(typepath, stat);
				if(stat.getType() == Stat::REGULAR_FILE)
					configuration.load(typepath);
			}
		}
		string cmpbase(prjconf.getProperty(Resources::RES_COMPONENT_PROPERTIES_FILE));
		if(cmpbase.empty())
			cmpbase = XakeComponent::DEFAULT_COMPONENT_PROPERTIES_FILE;
		string cmppath(Pathname::join(baseDirectory, cmpbase));
		if(Filesystem::access(cmppath, Filesystem::FILE_EXISTS)) {
			Filesystem::stat(cmppath, stat);
			if(stat.getType() == Stat::REGULAR_FILE)
				configuration.load(cmppath);
		}
	}

	XakeComponent::XakeComponent(const XakeComponent& component) : CompilerConfiguration(component),
			project(component.project), baseDirectory(component.baseDirectory), type(component.type),
			configuration(component.configuration), component(component.component),
			staticLinkerConfiguration(*this, true), dynamicLinkerConfiguration(*this, false) {}

	void XakeComponent::applyConfiguration(Compilation& compilation) {
		// internal API macro
		if(component) {
			const string& buildName = component->getInternalBuildName();
			if(!buildName.empty())
				compilation.defineMacro(buildName);
		}
		// XAKE_* macros
		compilation.defineMacro("XAKE_OS_LINUX", "1");
		compilation.defineMacro("XAKE_OS_WINDOWS", "2");
		// Note: The host OS of the build we're performing is the
		//       target OS of this process: This build of redbuild
		//       was built for buildTargetOS and thus can only run
		//       on that; making it *our* host.
		switch(buildTargetOS) {
			case OS_LINUX:
				compilation.defineMacro("XAKE_HOST_OS", "XAKE_OS_LINUX");
				break;
			case OS_WINDOWS:
				compilation.defineMacro("XAKE_HOST_OS", "XAKE_OS_WINDOWS");
				break;
			default:
				break;
		}
		compilation.defineMacro("XAKE_COMPILER_GCC", "1");
		compilation.defineMacro("XAKE_COMPILER", "XAKE_COMPILER_" + project.getCompilerName());
		// include directories
		if(component) {
			list<Component*> dependencies;
			component->getTransitiveDependencies(dependencies);
			const Language& cpp = *project.getCPPLanguage();
			list<Component*>::const_iterator cdbegin(dependencies.begin()), cdend(dependencies.end());
			for(; cdbegin != cdend; ++cdbegin) {
				string hedir((*cdbegin)->getHeaderExposeDirectory(cpp));
				if(!hedir.empty())
					compilation.addIncludeDirectory(hedir);
			}
		}
	}

}}}
