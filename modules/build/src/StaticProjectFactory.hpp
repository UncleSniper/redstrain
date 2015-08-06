#ifndef REDSTRAIN_MOD_BUILD_STATICPROJECTFACTORY_HPP
#define REDSTRAIN_MOD_BUILD_STATICPROJECTFACTORY_HPP

#include "ProjectFactory.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API StaticProjectFactory : public ProjectFactory {

	  private:
		std::string name;

	  public:
		StaticProjectFactory(const std::string&);
		StaticProjectFactory(const StaticProjectFactory&);

		inline const std::string& getName() const {
			return name;
		}

		void setName(const std::string&);

		virtual Project* newProject(const std::string&);

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_STATICPROJECTFACTORY_HPP */
