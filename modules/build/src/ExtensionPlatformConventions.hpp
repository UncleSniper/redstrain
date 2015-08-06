#ifndef REDSTRAIN_MOD_BUILD_EXTENSIONPLATFORMCONVENTIONS_HPP
#define REDSTRAIN_MOD_BUILD_EXTENSIONPLATFORMCONVENTIONS_HPP

#include "PlatformConventions.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API ExtensionPlatformConventions : public PlatformConventions {

	  private:
		std::string executableSuffix;
		std::string dynamicLibrarySuffix;

	  public:
		ExtensionPlatformConventions(const std::string&, const std::string&);
		ExtensionPlatformConventions(const ExtensionPlatformConventions&);

		inline const std::string& getExecutableSuffix() const {
			return executableSuffix;
		}

		inline const std::string& getDynamicLibrarySuffix() const {
			return dynamicLibrarySuffix;
		}

		void setExecutableSuffix(const std::string&);
		void setDynamicLibrarySuffix(const std::string&);

		virtual std::string makeNativeExecutableName(const std::string&) const;
		virtual std::string makeNativeDynamicLibraryName(const std::string&) const;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_EXTENSIONPLATFORMCONVENTIONS_HPP */
