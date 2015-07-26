#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEBUILDARTIFACTMAPPER_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEBUILDARTIFACTMAPPER_HPP

#include "../Component.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeBuildArtifactMapper : public Component::BuildArtifactMapper {

	  public:
		static const char *const DEFAULT_MODULE_BASENAME;
		static const char *const DEFAULT_TOOL_BASENAME;
		static const char *const DEFAULT_STATIC_FLAVOR_DECORATION;

	  private:
		XakeProject& project;

	  public:
		XakeBuildArtifactMapper(XakeProject&);
		XakeBuildArtifactMapper(const XakeBuildArtifactMapper&);

		inline XakeProject& getProject() {
			return project;
		}

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual std::string getTargetFileName(const Component&, const Language&, const Flavor&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEBUILDARTIFACTMAPPER_HPP */
