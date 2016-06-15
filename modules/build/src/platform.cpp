#include "platform.hpp"

using std::string;
using redengine::redmond::Architecture;
using redengine::redmond::OperatingSystem;
using redengine::redmond::OS_LINUX;
using redengine::redmond::ARCH_I686;
using redengine::redmond::OS_WINDOWS;
using redengine::redmond::ARCH_X86_64;

namespace redengine {
namespace build {

	REDSTRAIN_BUILD_API bool requiresPositionIndependentCode(Architecture architecture) {
		switch(architecture) {
			case ARCH_I686:
			// *sigh* GCC really doesn't know what it wants...
			// It used to complain about -fPIC when building on AMD64,
			// since "all code is position independent", but now it
			// insists that we use it.
			case ARCH_X86_64:
				return true;
			default:
				return false;
		}
	}

	REDSTRAIN_BUILD_API string decorateNativeExecutableName(const string& name, OperatingSystem os) {
		switch(os) {
			case OS_LINUX:
				return name;
			case OS_WINDOWS:
				return name + ".exe";
			default:
				return name;
		}
	}

	REDSTRAIN_BUILD_API string decorateNativeDynamicLibraryName(const string& name, OperatingSystem os) {
		switch(os) {
			case OS_LINUX:
				return name + ".so";
			case OS_WINDOWS:
				return name + ".dll";
			default:
				return name;
		}
	}

	REDSTRAIN_BUILD_API bool requiresInterlinkedLibraries(OperatingSystem os) {
		switch(os) {
			case OS_LINUX:
				return false;
			case OS_WINDOWS:
				return true;
			default:
				return false;
		}
	}

}}
