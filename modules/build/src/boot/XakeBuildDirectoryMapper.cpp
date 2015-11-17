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
	const char *const XakeBuildDirectoryMapper::DEFAULT_GENERATED_SOURCE_DIRECTORY = "gensrc";

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(XakeProject& project) : project(project) {}

	XakeBuildDirectoryMapper::XakeBuildDirectoryMapper(const XakeBuildDirectoryMapper& mapper)
			: BuildDirectoryMapper(mapper), project(mapper.project) {}

	void XakeBuildDirectoryMapper::getBuildDirectory(const Component&, const Language& language,
			const Flavor& flavor, string& directory, string& basename) {
		const Resources& configuration = project.getProjectConfiguration();
		if(&language == &project.getCPPLanguage()) {
			directory = configuration.getProperty(Resources::RES_BUILD_DIRECTORY);
			if(directory.empty())
				directory = XakeBuildDirectoryMapper::DEFAULT_BUILD_DIRECTORY;
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
			else {
				basename.clear();
				return;
			}
			basename = configuration.getProperty(id);
			if(basename.empty())
				basename = fdirDefault;
		}
		else if(
			&language == &project.getCodeTableDefinitionLanguage()
			|| &language == &project.getMessages16DefinitionLanguage()
		) {
			directory = configuration.getProperty(Resources::RES_BUILD_DIRECTORY);
			if(directory.empty())
				directory = XakeBuildDirectoryMapper::DEFAULT_BUILD_DIRECTORY;
		}
		else if(
			&language == &project.getBlobLanguage()
			|| &language == &project.getBlobAliasLanguage()
			|| &language == &project.getCodeTable16RegisterLanguage()
			|| &language == &project.getMessageBlobRegisterLanguage()
		) {
			directory = configuration.getProperty(Resources::RES_RSB_GENERATED_SOURCE_DIRECTORY);
			if(directory.empty())
				directory = XakeBuildDirectoryMapper::DEFAULT_GENERATED_SOURCE_DIRECTORY;
		}
		else {
			directory.clear();
			basename.clear();
		}
	}

	void XakeBuildDirectoryMapper::getHeaderExposeDirectory(const Component& component, const Language&,
			string& directory, string& basename) {
		directory = project.getProjectConfiguration().getProperty(Resources::RES_HEADER_EXPOSE_DIRECTORY);
		if(directory.empty())
			directory = XakeBuildDirectoryMapper::DEFAULT_HEADER_EXPOSE_DIRECTORY;
		basename = Pathname::join(project.getProjectName(), component.getName());
	}

}}}
