#include <redstrain/redmond/constants.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "../Compilation.hpp"
#include "XakeCompilerConfiguration.hpp"

using std::list;
using std::string;
using redengine::redmond::buildTargetOS;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;

namespace redengine {
namespace build {
namespace boot {

	XakeCompilerConfiguration::XakeCompilerConfiguration(XakeComponent& component) : component(component) {}

	XakeCompilerConfiguration::XakeCompilerConfiguration(const XakeCompilerConfiguration& configuration)
			: CompilerConfiguration(configuration), component(configuration.component) {}

	void XakeCompilerConfiguration::applyConfiguration(Compilation& compilation) {
		XakeProject& project = component.getProject();
		// internal API macro
		const string& buildMacro = component.getAPIBuildMacro();
		if(!buildMacro.empty())
			compilation.defineMacro(buildMacro);
		// XAKE_* macros
		compilation.defineMacro("XAKE_OS_LINUX", "1");
		compilation.defineMacro("XAKE_OS_WINDOWS", "2");
		// Note: The host OS of the build we're performing is the
		//       target OS of this process: This build of redbuild
		//       was built for buildTargetOS and thus can only run
		//       on that, making it *our* host.
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
		list<Component*> dependencies;
		component.getTransitiveDependencies(dependencies);
		const Language& cpp = project.getCPPLanguage();
		list<Component*>::const_iterator cdbegin(dependencies.begin()), cdend(dependencies.end());
		for(; cdbegin != cdend; ++cdbegin) {
			Component::ExposeDirectoryIterator edbegin, edend;
			(*cdbegin)->getHeaderExposeDirectories(cpp, edbegin, edend);
			for(; edbegin != edend; ++edbegin)
				compilation.addIncludeDirectory(*edbegin);
		}
	}

}}}
