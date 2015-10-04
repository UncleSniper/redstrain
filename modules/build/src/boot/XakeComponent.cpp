#include <redstrain/platform/Stat.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/platform/Filesystem.hpp>

#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeArtifactStageMapper.hpp"

using std::map;
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

	XakeComponent::XakeComponent(XakeProject& project, const string& baseDirectory, Component::Type type)
			: Component(type, "", baseDirectory), project(project),
			compilerConfiguration(*this), staticLinkerConfiguration(*this, true),
			dynamicLinkerConfiguration(*this, false), artifactStage("") {
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
		string name(configuration.getProperty(Resources::RES_COMPONENT_NAME));
		if(name.empty())
			setName(Pathname::basename(Pathname::tidy(baseDirectory)));
		else
			setName(name);
		string stagedir(prjconf.getProperty(Resources::RES_RSB_STAGE_DIRECTORY));
		if(stagedir.empty())
			stagedir = XakeArtifactStageMapper::DEFAULT_STAGE_DIRECTORY;
		stagedir = Pathname::tidy(Pathname::join(baseDirectory, stagedir));
		artifactStage.setDirectory(stagedir);
		artifactStage.setLabel(Pathname::stripPrefix(stagedir, Pathname::tidy(baseDirectory)));
	}

	XakeComponent::XakeComponent(const XakeComponent& component) : Component(component),
			project(component.project), configuration(component.configuration), compilerConfiguration(*this),
			staticLinkerConfiguration(*this, true), dynamicLinkerConfiguration(*this, false),
			artifactStage(component.artifactStage) {}

	string XakeComponent::getComponentGuard() const {
		string guard(configuration.getProperty(Resources::RES_COMPONENT_GUARD));
		if(guard.empty())
			guard = XakeUtils::slugifyMacro(getName());
		return guard;
	}

	string XakeComponent::getAPIBuildMacro() const {
		string tpl(configuration.getProperty(Resources::RES_INTERNAL_API_MACRO));
		if(tpl.empty())
			tpl = project.getProjectConfiguration().getProperty(Resources::RES_INTERNAL_API_MACRO);
		map<string, string> variables;
		variables["project"] = project.getProjectGuard();
		variables["module"] = getComponentGuard();
		return XakeUtils::subst(tpl, variables);
	}

}}}
