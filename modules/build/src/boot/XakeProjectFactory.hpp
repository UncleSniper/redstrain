#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECTFACTORY_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECTFACTORY_HPP

#include "../DirectoryProjectFactory.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeProjectFactory : public DirectoryProjectFactory {

	  private:
		const XakeProject& project;

	  public:
		XakeProjectFactory(const XakeProject&);
		XakeProjectFactory(const XakeProjectFactory&);

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual Project* newProject(const std::string&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECTFACTORY_HPP */
