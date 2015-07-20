#include <redstrain/platform/Pathname.hpp>

#include "XakeProject.hpp"
#include "XakeBuildDirectoryMapper.hpp"

using std::string;
using redengine::platform::Pathname;

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeBuildDirectoryMapper::DEFAULT_HEADER_EXPOSE_DIRECTORY = "include";

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(const XakeProject& project) : project(project) {}

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(const XakeBuildDirectoryMapper& mapper)
			: BuildDirectoryMapper(mapper), project(mapper.project) {}

	string XakeBuildDirectoryMapper::getBuildDirectory(const Component&, const Language&, const Flavor&) {
		//TODO: RES_BUILD_DIRECTORY + RES_STATIC_BUILD_DIRECTORY/RES_DYNAMIC_BUILD_DIRECTORY
		return "";
	}

	string XakeBuildDirectoryMapper::getHeaderExposeDirectory(const Component& component, const Language&) {
		string directory(project.getProjectConfiguration().getProperty(Resources::RES_HEADER_EXPOSE_DIRECTORY));
		if(directory.empty())
			directory = XakeBuildDirectoryMapper::DEFAULT_HEADER_EXPOSE_DIRECTORY;
		directory = Pathname::join(directory, project.getProjectName());
		return Pathname::join(directory, component.getName());
	}

}}}
