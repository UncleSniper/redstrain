#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEOBJECTFILELANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEOBJECTFILELANGUAGE_HPP

#include "../NativeObjectFileLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API XakeObjectFileLanguage : public NativeObjectFileLanguage {

	  private:
		const XakeProject& project;

	  protected:
		virtual LinkerConfiguration& getLinkerConfiguration(const Flavor&, const Component&);

	  public:
		XakeObjectFileLanguage(XakeProject&);
		XakeObjectFileLanguage(const XakeObjectFileLanguage&);

		inline const XakeProject& getProject() const {
			return project;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEOBJECTFILELANGUAGE_HPP */
