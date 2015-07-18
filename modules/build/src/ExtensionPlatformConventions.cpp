#include "ExtensionPlatformConventions.hpp"

using std::string;

namespace redengine {
namespace build {

	ExtensionPlatformConventions::ExtensionPlatformConventions(const string& executableSuffix,
			const string& dynamicLibrarySuffix) : executableSuffix(executableSuffix),
			dynamicLibrarySuffix(dynamicLibrarySuffix) {}

	ExtensionPlatformConventions::ExtensionPlatformConventions(const ExtensionPlatformConventions& conventions)
			: PlatformConventions(conventions), executableSuffix(conventions.executableSuffix),
			dynamicLibrarySuffix(conventions.dynamicLibrarySuffix) {}

	void ExtensionPlatformConventions::setExecutableSuffix(const string& suffix) {
		executableSuffix = suffix;
	}

	void ExtensionPlatformConventions::setDynamicLibrarySuffix(const string& suffix) {
		dynamicLibrarySuffix = suffix;
	}

	string ExtensionPlatformConventions::makeNativeExecutableName(const string& basename) const {
		return basename + executableSuffix;
	}

	string ExtensionPlatformConventions::makeNativeDynamicLibraryName(const string& basename) const {
		return basename + dynamicLibrarySuffix;
	}

}}
