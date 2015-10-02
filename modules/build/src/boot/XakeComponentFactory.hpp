#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTFACTORY_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTFACTORY_HPP

#include "../ComponentFactory.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeComponentFactory : public ComponentFactory {

	  private:
		XakeProject& project;

	  public:
		XakeComponentFactory(XakeProject&);
		XakeComponentFactory(const XakeComponentFactory&);

		inline XakeProject& getProject() {
			return project;
		}

		inline const XakeProject& getProject() const {
			return project;
		}

		virtual Component* newComponent(const Project&, const std::string&, Component::Type);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKECOMPONENTFACTORY_HPP */
