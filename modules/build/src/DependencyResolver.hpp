#ifndef REDSTRAIN_MOD_BUILD_DEPENDENCYRESOLVER_HPP
#define REDSTRAIN_MOD_BUILD_DEPENDENCYRESOLVER_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Project;
	class Component;

	class REDSTRAIN_BUILD_API DependencyResolver {

	  public:
		DependencyResolver();
		DependencyResolver(const DependencyResolver&);
		virtual ~DependencyResolver();

		virtual void resolveDependencies(const Project&, Component&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_DEPENDENCYRESOLVER_HPP */
