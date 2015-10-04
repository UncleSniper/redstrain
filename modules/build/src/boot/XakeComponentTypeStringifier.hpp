#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTTYPESTRINGIFIER_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTTYPESTRINGIFIER_HPP

#include "../ComponentTypeStringifier.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeComponentTypeStringifier : public ComponentTypeStringifier {

	  private:
		const XakeProject& project;

	  public:
		XakeComponentTypeStringifier(const XakeProject&);
		XakeComponentTypeStringifier(const XakeComponentTypeStringifier&);

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual std::string stringifyComponentType(Component::Type);
	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTTYPESTRINGIFIER_HPP */
