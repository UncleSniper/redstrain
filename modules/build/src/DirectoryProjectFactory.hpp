#ifndef REDSTRAIN_MOD_BUILD_DIRECTORYPROJECTFACTORY_HPP
#define REDSTRAIN_MOD_BUILD_DIRECTORYPROJECTFACTORY_HPP

#include "ProjectFactory.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API DirectoryProjectFactory : public ProjectFactory {

	  public:
		static const char *const DEFAULT_NAME;

	  private:
		std::string defaultName;

	  public:
		DirectoryProjectFactory(const std::string& = "");
		DirectoryProjectFactory(const DirectoryProjectFactory&);

		inline const std::string& getDefaultName() const {
			return defaultName;
		}

		void setDefaultName(const std::string& = "");

		virtual Project* newProject(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DIRECTORYPROJECTFACTORY_HPP */
