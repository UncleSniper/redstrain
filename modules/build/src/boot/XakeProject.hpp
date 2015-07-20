#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP

#include "Resources.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API XakeProject {

	  private:
		const std::string baseDirectory;
		Resources configuration;

	  public:
		XakeProject(const std::string&);
		XakeProject(const XakeProject&);

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

		inline Resources& getProjectConfiguration() {
			return configuration;
		}

		inline const Resources& getProjectConfiguration() const {
			return configuration;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP */
