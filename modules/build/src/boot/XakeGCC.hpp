#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEGCC_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEGCC_HPP

#include "../GCC.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeGCC : public GCC {

	  public:
		static const char *const DEFAULT_EXECUTABLE;
		static const char *const DEFAULT_AR_EXECUTABLE;

	  private:
		const XakeProject& project;

	  public:
		XakeGCC(const XakeProject&, const std::string&, const std::string&,
				redmond::Architecture, redmond::OperatingSystem);
		XakeGCC(const XakeGCC&);

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual Compilation* newCompilation(const std::string&, Compilation::CompileMode);
		virtual Linkage* newLinkage(const std::string&, Linkage::LinkMode);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEGCC_HPP */
