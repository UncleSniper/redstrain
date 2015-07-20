#include "XakeValveInjector.hpp"

namespace redengine {
namespace build {
namespace boot {

	XakeValveInjector::XakeValveInjector(const XakeProject& project) : project(project) {}

	XakeValveInjector::XakeValveInjector(const XakeValveInjector& injector)
			: ValveInjector(injector), project(injector.project) {}

	void XakeValveInjector::injectIntoTrigger(Trigger&, const Component&, const Language*,
			const Flavor&, BuildContext&) {
		//TODO
	}

}}}
