#include <redstrain/platform/Pathname.hpp>

#include "XakeProject.hpp"
#include "XakeBuildDirectoryMapper.hpp"

using std::string;
using redengine::platform::Pathname;

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeBuildDirectoryMapper::DEFAULT_HEADER_EXPOSE_DIRECTORY = "include";
	const char *const XakeBuildDirectoryMapper::DEFAULT_BUILD_DIRECTORY = "build";
	const char *const XakeBuildDirectoryMapper::DEFAULT_STATIC_BUILD_DIRECTORY = "static";
	const char *const XakeBuildDirectoryMapper::DEFAULT_DYNAMIC_BUILD_DIRECTORY = "dynamic";

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(XakeProject& project) : project(project) {}

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(const XakeBuildDirectoryMapper& mapper)
			: BuildDirectoryMapper(mapper), project(mapper.project) {}

	string XakeBuildDirectoryMapper::getBuildDirectory(const Component&, const Language& language,
			const Flavor& flavor) {
		if(&language == project.getCPPLanguage()) {
			const Resources& configuration = project.getProjectConfiguration();
			string bdir(configuration.getProperty(Resources::RES_BUILD_DIRECTORY));
			if(bdir.empty())
				bdir = XakeBuildDirectoryMapper::DEFAULT_BUILD_DIRECTORY;
			Resources::ID id;
			const char* fdirDefault;
			if(flavor == Flavor::STATIC) {
				id = Resources::RES_STATIC_BUILD_DIRECTORY;
				fdirDefault = XakeBuildDirectoryMapper::DEFAULT_STATIC_BUILD_DIRECTORY;
			}
			else if(flavor == Flavor::DYNAMIC) {
				id = Resources::RES_DYNAMIC_BUILD_DIRECTORY;
				fdirDefault = XakeBuildDirectoryMapper::DEFAULT_DYNAMIC_BUILD_DIRECTORY;
			}
			else
				return bdir;
			string fdir(configuration.getProperty(id));
			if(fdir.empty())
				fdir = fdirDefault;
			return Pathname::join(bdir, fdir);
		}
		else
			return "";
	}

	void XakeBuildDirectoryMapper::getHeaderExposeDirectory(const Component& component, const Language&,
			string& directory, string& basename) {
		directory = project.getProjectConfiguration().getProperty(Resources::RES_HEADER_EXPOSE_DIRECTORY);
		if(directory.empty())
			directory = XakeBuildDirectoryMapper::DEFAULT_HEADER_EXPOSE_DIRECTORY;
		basename = Pathname::join(project.getProjectName(), component.getName());
	}

}}}
