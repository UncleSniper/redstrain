#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEDEPENDENCYRESOLVER_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEDEPENDENCYRESOLVER_HPP

#include "../DependencyResolver.hpp"

namespace redengine {
namespace build {
namespace boot {

	class REDSTRAIN_BUILD_API XakeDependencyResolver : public DependencyResolver {

	  public:
		XakeDependencyResolver();
		XakeDependencyResolver(const XakeDependencyResolver&);

		virtual void resolveDependencies(const Project&, Component&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEDEPENDENCYRESOLVER_HPP */
