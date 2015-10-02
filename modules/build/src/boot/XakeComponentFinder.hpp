#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTFINDER_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTFINDER_HPP

#include "../DirectoryComponentFinder.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeComponentFinder : public DirectoryComponentFinder {

	  public:
		static const char *const DEFAULT_LIBRARIES_DIRECTORY;
		static const char *const DEFAULT_BINARIES_DIRECTORY;
		static const char *const DEFAULT_BLOBS_DIRECTORY;

	  private:
		const XakeProject& project;
		std::string defaultLibrariesDirectory, defaultBinariesDirectory, defaultBlobsDirectory;

	  public:
		XakeComponentFinder(const XakeProject&);
		XakeComponentFinder(const XakeComponentFinder&);

		inline const XakeProject& getProject() const {
			return project;
		}

		inline const std::string& getDefaultLibrariesDirectory() const {
			return defaultLibrariesDirectory;
		}

		void setDefaultLibrariesDirectory(const std::string&);

		inline const std::string& getDefaultBinariesDirectory() const {
			return defaultBinariesDirectory;
		}

		void setDefaultBinariesDirectory(const std::string&);

		inline const std::string& getDefaultBlobsDirectory() const {
			return defaultBlobsDirectory;
		}

		void setDefaultBlobsDirectory(const std::string&);

		virtual void findComponents(const Project&, ProjectBuilder&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTFINDER_HPP */
