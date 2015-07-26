#ifndef REDSTRAIN_MOD_BUILD_PLATFORM_HPP
#define REDSTRAIN_MOD_BUILD_PLATFORM_HPP

#include <string>
#include <redstrain/redmond/constants.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	REDSTRAIN_BUILD_API bool requiresPositionIndependentCode(redmond::Architecture);
	REDSTRAIN_BUILD_API std::string decorateNativeExecutableName(const std::string&, redmond::OperatingSystem);
	REDSTRAIN_BUILD_API std::string decorateNativeDynamicLibraryName(const std::string&, redmond::OperatingSystem);
	REDSTRAIN_BUILD_API bool requiresInterlinkedLibraries(redmond::OperatingSystem);

}}

#endif /* REDSTRAIN_MOD_BUILD_PLATFORM_HPP */
