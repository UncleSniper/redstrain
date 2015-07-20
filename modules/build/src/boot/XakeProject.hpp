#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP

#include <map>

#include "Resources.hpp"
#include "../Component.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeComponent;

	class REDSTRAIN_BUILD_API XakeProject {

	  private:
		typedef std::map<Component*, XakeComponent*> Components;
		typedef Components::iterator ComponentIterator;
		typedef Components::const_iterator ConstComponentIterator;

	  private:
		const std::string baseDirectory;
		Resources configuration;
		Components components;

	  public:
		XakeProject(const std::string&);
		XakeProject(const XakeProject&);
		~XakeProject();

		inline const std::string& getBaseDirectory() const {
			return baseDirectory;
		}

		inline Resources& getProjectConfiguration() {
			return configuration;
		}

		inline const Resources& getProjectConfiguration() const {
			return configuration;
		}

		XakeComponent* getComponent(Component*) const;
		bool addComponent(Component*, XakeComponent*);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEPROJECT_HPP */
