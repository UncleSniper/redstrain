#include <redstrain/util/Unref.hpp>

#include "../Language.hpp"
#include "XakeProject.hpp"
#include "../StaticValve.hpp"
#include "../BuildContext.hpp"
#include "XakeValveInjector.hpp"

using redengine::util::Unref;

namespace redengine {
namespace build {
namespace boot {

	XakeValveInjector::XakeValveInjector(XakeProject& project) : project(project) {}

	XakeValveInjector::XakeValveInjector(const XakeValveInjector& injector)
			: ValveInjector(injector), project(injector.project) {}

	void XakeValveInjector::injectIntoTrigger(Trigger& trigger, const Component& component, Language* language,
			const Flavor& flavor, BuildContext& context) {
		bool isClean;
		if(language)
			isClean = flavor == language->getCleanFlavor();
		else
			isClean = flavor == Flavor::CLEAN;
		if(isClean)
			trigger.addValve(project.getCleanValve(context));
		else
			trigger.addValve(project.getBuildValve(context));
		if(component.getType() == Component::EXECUTABLE)
			trigger.addValve(project.getToolsValve(context));
		else
			trigger.addValve(project.getModulesValve(context));
		if(flavor == Flavor::STATIC)
			trigger.addValve(project.getStaticValve(context));
		else if(flavor == Flavor::DYNAMIC)
			trigger.addValve(project.getDynamicValve(context));
		//TODO: component-specific
		//TODO: XakeProject::makeValveGroups()
	}

}}}
