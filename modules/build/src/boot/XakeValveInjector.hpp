#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEVALVEINJECTOR_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEVALVEINJECTOR_HPP

#include "../Component.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeValveInjector : public Component::ValveInjector {

	  private:
		const XakeProject& project;

	  public:
		XakeValveInjector(const XakeProject&);
		XakeValveInjector(const XakeValveInjector&);

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual void injectIntoTrigger(Trigger&, const Component&, const Language*,
				const Flavor&, BuildContext&) = 0;

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEVALVEINJECTOR_HPP */
