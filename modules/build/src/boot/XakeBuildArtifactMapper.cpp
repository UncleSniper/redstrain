#include "XakeUtils.hpp"
#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeBuildArtifactMapper.hpp"

using std::map;
using std::string;

namespace redengine {
namespace build {
namespace boot {

	const char *const XakeBuildArtifactMapper::DEFAULT_MODULE_BASENAME = "%project%-%module%";
	const char *const XakeBuildArtifactMapper::DEFAULT_TOOL_BASENAME = "%project%%tool%";
	const char *const XakeBuildArtifactMapper::DEFAULT_STATIC_FLAVOR_DECORATION = "%-static";

	XakeBuildArtifactMapper::XakeBuildArtifactMapper(XakeProject& project) : project(project) {}

	XakeBuildArtifactMapper::XakeBuildArtifactMapper(const XakeBuildArtifactMapper& mapper)
			: BuildArtifactMapper(mapper), project(mapper.project) {}

	string XakeBuildArtifactMapper::getTargetFileName(const Component& component, const Language& language,
			const Flavor& flavor) {
		if(&language == &project.getObjectFileLanguage()) {
			const XakeComponent* xcomponent = dynamic_cast<const XakeComponent*>(&component);
			string cname;
			if(xcomponent)
				cname = xcomponent->getComponentConfiguration().getProperty(Resources::RES_COMPONENT_NAME);
			if(cname.empty())
				cname = component.getName();
			map<string, string> variables;
			variables["project"] = project.getProjectName();
			string tpl;
			if(component.getType() == Component::EXECUTABLE) {
				tpl = project.getProjectConfiguration().getProperty(Resources::RES_TOOL_BASENAME);
				if(tpl.empty())
					tpl = XakeBuildArtifactMapper::DEFAULT_TOOL_BASENAME;
				variables["tool"] = cname;
			}
			else {
				tpl = project.getProjectConfiguration().getProperty(Resources::RES_MODULE_BASENAME);
				if(tpl.empty())
					tpl = XakeBuildArtifactMapper::DEFAULT_MODULE_BASENAME;
				variables["module"] = cname;
			}
			string basename(XakeUtils::subst(tpl, variables));
			if(flavor == Flavor::STATIC) {
				string decoration(project.getProjectConfiguration().getProperty(
						Resources::RES_STATIC_FLAVOR_DECORATION));
				if(decoration.empty())
					decoration = XakeBuildArtifactMapper::DEFAULT_STATIC_FLAVOR_DECORATION;
				string::size_type pos = decoration.find('%');
				if(pos != string::npos)
					basename = decoration.substr(static_cast<string::size_type>(0u), pos) + basename
							+ decoration.substr(pos + static_cast<string::size_type>(1u));
			}
			return basename;
		}
		return project.getProjectName() + '-' + component.getName();
	}

}}}
