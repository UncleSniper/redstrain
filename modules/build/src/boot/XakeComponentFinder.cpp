#include "XakeProject.hpp"
#include "XakeComponentFinder.hpp"

using std::string;

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeComponentFinder::DEFAULT_LIBRARIES_DIRECTORY = "modules";
	const char *const XakeComponentFinder::DEFAULT_BINARIES_DIRECTORY = "tools";
	const char *const XakeComponentFinder::DEFAULT_BLOBS_DIRECTORY = "data";

	XakeComponentFinder::XakeComponentFinder(const XakeProject& project) : project(project) {}

	XakeComponentFinder::XakeComponentFinder(const XakeComponentFinder& finder)
			: DirectoryComponentFinder(finder), project(finder.project),
			defaultLibrariesDirectory(finder.defaultLibrariesDirectory),
			defaultBinariesDirectory(finder.defaultBinariesDirectory),
			defaultBlobsDirectory(finder.defaultBlobsDirectory) {}

	void XakeComponentFinder::setDefaultLibrariesDirectory(const string& directory) {
		defaultLibrariesDirectory = directory;
	}

	void XakeComponentFinder::setDefaultBinariesDirectory(const string& directory) {
		defaultBinariesDirectory = directory;
	}

	void XakeComponentFinder::setDefaultBlobsDirectory(const string& directory) {
		defaultBlobsDirectory = directory;
	}

	void XakeComponentFinder::findComponents(const Project& buildingProject, ProjectBuilder& builder) {
		const Resources& configuration = project.getProjectConfiguration();
		string directory(configuration.getProperty(Resources::RES_LIBRARIES_DIRECTORY));
		if(directory.empty()) {
			if(defaultLibrariesDirectory.empty())
				directory = XakeComponentFinder::DEFAULT_LIBRARIES_DIRECTORY;
			else
				directory = defaultLibrariesDirectory;
		}
		addComponentsDirectory(directory, Component::LIBRARY);
		directory = configuration.getProperty(Resources::RES_BINARIES_DIRECTORY);
		if(directory.empty()) {
			if(defaultBinariesDirectory.empty())
				directory = XakeComponentFinder::DEFAULT_BINARIES_DIRECTORY;
			else
				directory = defaultBinariesDirectory;
		}
		addComponentsDirectory(directory, Component::EXECUTABLE);
		directory = configuration.getProperty(Resources::RES_RSB_BLOBS_DIRECTORY);
		if(directory.empty()) {
			if(defaultBlobsDirectory.empty())
				directory = XakeComponentFinder::DEFAULT_BLOBS_DIRECTORY;
			else
				directory = defaultBlobsDirectory;
		}
		addComponentsDirectory(directory, Component::BLOB);
		DirectoryComponentFinder::findComponents(buildingProject, builder);
	}

}}}
