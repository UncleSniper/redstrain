#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEVALVEINJECTOR_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEVALVEINJECTOR_HPP

#include "../Component.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeValveInjector : public Component::ValveInjector {

	  private:
		XakeProject& project;

	  public:
		XakeValveInjector(XakeProject&);
		XakeValveInjector(const XakeValveInjector&);

		inline XakeProject& getProject() {
			return project;
		}

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual void injectIntoTrigger(Trigger&, const Component&, Language*, const Flavor&, BuildContext&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEVALVEINJECTOR_HPP */
