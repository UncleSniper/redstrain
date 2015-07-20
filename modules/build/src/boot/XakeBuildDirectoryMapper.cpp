#include "XakeProject.hpp"
#include "XakeBuildDirectoryMapper.hpp"

using std::string;

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeBuildDirectoryMapper::DEFAULT_HEADER_EXPOSE_DIRECTORY = "include";

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(const XakeProject& project) : project(project) {}

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(const XakeBuildDirectoryMapper& mapper)
			: BuildDirectoryMapper(mapper), project(mapper.project) {}

	string XakeBuildDirectoryMapper::getBuildDirectory(const Component&, const Language&, const Flavor&) {
		//TODO
		return "";
	}

	string XakeBuildDirectoryMapper::getHeaderExposeDirectory(const Component&, const Language&) {
		string directory(project.getProjectConfiguration().getProperty(Resources::RES_HEADER_EXPOSE_DIRECTORY));
		if(directory.empty())
			return XakeBuildDirectoryMapper::DEFAULT_HEADER_EXPOSE_DIRECTORY;
		return directory;
	}

}}}
