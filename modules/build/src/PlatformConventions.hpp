#ifndef REDSTRAIN_MOD_BUILD_PLATFORMCONVENTIONS_HPP
#define REDSTRAIN_MOD_BUILD_PLATFORMCONVENTIONS_HPP

#include <string>
#include <redstrain/redmond/constants.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API PlatformConventions {

	  public:
		PlatformConventions();
		PlatformConventions(const PlatformConventions&);
		virtual ~PlatformConventions();

		virtual std::string makeNativeExecutableName(const std::string&) const = 0;
		virtual std::string makeNativeDynamicLibraryName(const std::string&) const = 0;

		static const PlatformConventions& getDefaultConventions(redmond::OperatingSystem);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_PLATFORMCONVENTIONS_HPP */
