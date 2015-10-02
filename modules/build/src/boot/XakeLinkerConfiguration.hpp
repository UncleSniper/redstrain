#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKELINKERCONFIGURATION_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKELINKERCONFIGURATION_HPP

#include "../LinkerConfiguration.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeComponent;

	class REDSTRAIN_BUILD_API XakeLinkerConfiguration : public LinkerConfiguration {

	  private:
		XakeComponent& component;
		const bool forStatic;

	  public:
		XakeLinkerConfiguration(XakeComponent&, bool);
		XakeLinkerConfiguration(const XakeLinkerConfiguration&);

		inline XakeComponent& getComponent() {
			return component;
		}

		inline const XakeComponent& getComponent() const {
			return component;
		}

		inline bool isForStatic() const {
			return forStatic;
		}

		virtual void applyConfiguration(Linkage&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKELINKERCONFIGURATION_HPP */
