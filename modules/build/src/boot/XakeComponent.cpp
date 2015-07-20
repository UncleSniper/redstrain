#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "XakeProject.hpp"
#include "XakeComponent.hpp"

using std::string;
using redengine::platform::Stat;
using redengine::platform::Pathname;
using redengine::platform::Filesystem;

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeComponent::DEFAULT_COMMON_MODULES_PROPERTIES_FILE = "modules.properties";
	const char *const XakeComponent::DEFAULT_COMMON_TOOLS_PROPERTIES_FILE = "tools.properties";
	const char *const XakeComponent::DEFAULT_COMPONENT_PROPERTIES_FILE = "component.properties";

	XakeComponent::XakeComponent(const XakeProject& project, const string& baseDirectory, Component::Type type)
			: project(project), baseDirectory(baseDirectory), type(type) {
		Resources::ID typeres;
		const char* typedefault;
		switch(type) {
			case Component::LIBRARY:
				typeres = Resources::RES_COMMON_MODULES_PROPERTIES_FILE;
				typedefault = XakeComponent::DEFAULT_COMMON_MODULES_PROPERTIES_FILE;
				break;
			case Component::EXECUTABLE:
				typeres = Resources::RES_COMMON_TOOLS_PROPERTIES_FILE;
				typedefault = XakeComponent::DEFAULT_COMMON_TOOLS_PROPERTIES_FILE;
				break;
			default:
				typeres = Resources::RES__LAST;
				typedefault = NULL;
				break;
		}
		const Resources& prjconf = project.getProjectConfiguration();
		Stat stat;
		if(typedefault) {
			string typedir(Pathname::join(baseDirectory, Pathname::PARENT_DIRECTORY));
			string typebase(prjconf.getProperty(typeres));
			if(typebase.empty())
				typebase = typedefault;
			string typepath(Pathname::join(typedir, typebase));
			if(Filesystem::access(typepath, Filesystem::FILE_EXISTS)) {
				Filesystem::stat(typepath, stat);
				if(stat.getType() == Stat::REGULAR_FILE)
					configuration.load(typepath);
			}
		}
		string cmpbase(prjconf.getProperty(Resources::RES_COMPONENT_PROPERTIES_FILE));
		if(cmpbase.empty())
			cmpbase = XakeComponent::DEFAULT_COMPONENT_PROPERTIES_FILE;
		string cmppath(Pathname::join(baseDirectory, cmpbase));
		if(Filesystem::access(cmppath, Filesystem::FILE_EXISTS)) {
			Filesystem::stat(cmppath, stat);
			if(stat.getType() == Stat::REGULAR_FILE)
				configuration.load(cmppath);
		}
	}

	XakeComponent::XakeComponent(const XakeComponent& component)
			: project(component.project), baseDirectory(component.baseDirectory), type(component.type),
			configuration(component.configuration) {}

}}}
