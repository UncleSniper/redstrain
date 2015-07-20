#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEBUILDDIRECTORYMAPPER_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEBUILDDIRECTORYMAPPER_HPP

#include "../Component.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeBuildDirectoryMapper : public Component::BuildDirectoryMapper {

	  public:
		static const char *const DEFAULT_HEADER_EXPOSE_DIRECTORY;

	  private:
		const XakeProject& project;

	  public:
		XakeBuildDirectoryMapper(const XakeProject&);
		XakeBuildDirectoryMapper(const XakeBuildDirectoryMapper&);

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual std::string getBuildDirectory(const Component&, const Language&, const Flavor&);
		virtual std::string getHeaderExposeDirectory(const Component&, const Language&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEBUILDDIRECTORYMAPPER_HPP */
