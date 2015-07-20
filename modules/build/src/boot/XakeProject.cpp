#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/redmond/constants.hpp>

#include "XakeProject.hpp"

using std::string;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;
using redengine::redmond::OS_LINUX;
using redengine::redmond::OS_WINDOWS;
using redengine::redmond::buildTargetOS;

namespace redengine {
namespace build {
namespace boot {

	XakeProject::XakeProject(const string& baseDirectory) : baseDirectory(baseDirectory) {
		configuration.load(Resources::DFL_DEFAULTS);
		switch(buildTargetOS) {
			case OS_LINUX:
				configuration.load(Resources::DFL_LINUX_DEFAULTS);
				break;
			case OS_WINDOWS:
				configuration.load(Resources::DFL_WINDOWS_DEFAULTS);
				break;
			default:
				break;
		}
		Stat stat;
		string pcpath(Pathname::join(baseDirectory, "project.properties"));
		if(Filesystem::access(pcpath, Filesystem::FILE_EXISTS)) {
			Filesystem::stat(pcpath, stat);
			if(stat.getType() == Stat::REGULAR_FILE)
				configuration.load(pcpath);
		}
		string scpath(Pathname::join(baseDirectory, "site.properties"));
		if(Filesystem::access(scpath, Filesystem::FILE_EXISTS)) {
			Filesystem::stat(scpath, stat);
			if(stat.getType() == Stat::REGULAR_FILE)
				configuration.load(scpath);
		}
	}

	XakeProject::XakeProject(const XakeProject& project)
			: baseDirectory(project.baseDirectory), configuration(project.configuration) {}

}}}
