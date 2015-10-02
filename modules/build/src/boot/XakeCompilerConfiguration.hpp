#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPILERCONFIGURATION_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPILERCONFIGURATION_HPP

#include "../CompilerConfiguration.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeComponent;

	class REDSTRAIN_BUILD_API XakeCompilerConfiguration : public CompilerConfiguration {

	  private:
		XakeComponent& component;

	  public:
		XakeCompilerConfiguration(XakeComponent&);
		XakeCompilerConfiguration(const XakeCompilerConfiguration&);

		inline XakeComponent& getComponent() {
			return component;
		}

		inline const XakeComponent& getComponent() const {
			return component;
		}

		virtual void applyConfiguration(Compilation&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPILERCONFIGURATION_HPP */
