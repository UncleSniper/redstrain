#include "ExtensionPlatformConventions.hpp"

using redengine::redmond::OperatingSystem;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;

namespace redengine {
namespace build {

	PlatformConventions::PlatformConventions() {}

	PlatformConventions::PlatformConventions(const PlatformConventions&) {}

	PlatformConventions::~PlatformConventions() {}

	static ExtensionPlatformConventions identityEPC("", ""), linuxEPC("", ".so"), windowsEPC(".exe", ".dll");

	const PlatformConventions& PlatformConventions::getDefaultConventions(OperatingSystem os) {
		switch(os) {
			case OS_LINUX:
				return linuxEPC;
			case OS_WINDOWS:
				return windowsEPC;
			default:
				return identityEPC;
		}
	}

}}
