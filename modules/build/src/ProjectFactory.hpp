#ifndef REDSTRAIN_MOD_BUILD_PROJECTFACTORY_HPP
#define REDSTRAIN_MOD_BUILD_PROJECTFACTORY_HPP

#include <string>

#include "api.hpp"

namespace redengine {
namespace build {

	class Project;

	class REDSTRAIN_BUILD_API ProjectFactory {

	  public:
		ProjectFactory();
		ProjectFactory(const ProjectFactory&);
		virtual ~ProjectFactory();

		virtual Project* newProject(const std::string&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_PROJECTFACTORY_HPP */
