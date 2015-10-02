#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECPPLANGUAGE_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECPPLANGUAGE_HPP

#include "../CPPLanguage.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeCPPLanguage : public CPPLanguage {

	  private:
		const XakeProject& project;

	  protected:
		virtual CompilerConfiguration& getCompilerConfiguration(const Flavor&, const Component&);

	  public:
		XakeCPPLanguage(XakeProject&);
		XakeCPPLanguage(const XakeCPPLanguage&);

		inline const XakeProject& getProject() const {
			return project;
		}

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECPPLANGUAGE_HPP */
