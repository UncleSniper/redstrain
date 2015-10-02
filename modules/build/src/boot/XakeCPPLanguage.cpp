#include "XakeProject.hpp"
#include "XakeComponent.hpp"
#include "XakeCPPLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	XakeCPPLanguage::XakeCPPLanguage(XakeProject& project) : CPPLanguage(project.getCompiler()), project(project) {}

	XakeCPPLanguage::XakeCPPLanguage(const XakeCPPLanguage& cpp) : CPPLanguage(cpp), project(cpp.project) {}

	CompilerConfiguration& XakeCPPLanguage::getCompilerConfiguration(const Flavor& transformFlavor,
			const Component& component) {
		const XakeComponent* xcomponent = dynamic_cast<const XakeComponent*>(&component);
		if(xcomponent)
			return xcomponent->getCompilerConfiguration();
		else
			return CPPLanguage::getCompilerConfiguration(transformFlavor, component);
	}

}}}
