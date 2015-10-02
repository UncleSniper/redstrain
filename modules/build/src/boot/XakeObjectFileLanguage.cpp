#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeObjectFileLanguage.hpp"

using std::string;

namespace redengine {
namespace build {
namespace boot {

	XakeObjectFileLanguage::XakeObjectFileLanguage(XakeProject& project)
			: NativeObjectFileLanguage(project.getLinker()), project(project) {}

	XakeObjectFileLanguage::XakeObjectFileLanguage(const XakeObjectFileLanguage& language)
			: NativeObjectFileLanguage(language), project(language.project) {}

	LinkerConfiguration& XakeObjectFileLanguage::getLinkerConfiguration(const Flavor& transformFlavor,
			const Component& component) {
		const XakeComponent* xcomponent = dynamic_cast<const XakeComponent*>(&component);
		if(xcomponent) {
			if(transformFlavor == Flavor::STATIC)
				return xcomponent->getStaticLinkerConfiguration();
			if(transformFlavor == Flavor::DYNAMIC)
				return xcomponent->getDynamicLinkerConfiguration();
		}
		return NativeObjectFileLanguage::getLinkerConfiguration(transformFlavor, component);
	}

}}}
