#ifndef REDSTRAIN_MOD_BUILD_COMPONENTFINDER_HPP
#define REDSTRAIN_MOD_BUILD_COMPONENTFINDER_HPP

#include "api.hpp"

namespace redengine {
namespace build {

	class Project;
	class ProjectBuilder;

	class REDSTRAIN_BUILD_API ComponentFinder {

	  public:
		ComponentFinder();
		ComponentFinder(const ComponentFinder&);
		virtual ~ComponentFinder();

		virtual void findComponents(const Project&, ProjectBuilder&) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_COMPONENTFINDER_HPP */
