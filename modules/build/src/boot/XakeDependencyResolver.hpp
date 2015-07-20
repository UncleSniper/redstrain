#ifndef REDSTRAIN_MOD_BUILD_BOOT_XAKEDEPENDENCYRESOLVER_HPP
#define REDSTRAIN_MOD_BUILD_BOOT_XAKEDEPENDENCYRESOLVER_HPP

#include "../ProjectBuilder.hpp"

namespace redengine {
namespace build {
namespace boot {

	class XakeProject;

	class REDSTRAIN_BUILD_API XakeDependencyResolver : public ProjectBuilder::DependencyResolver {

	  private:
		const XakeProject& project;

	  public:
		XakeDependencyResolver(const XakeProject&);
		XakeDependencyResolver(const XakeDependencyResolver&);

		virtual void resolveDependencies(const Project&, Component&);

	};

}}}

#endif /* REDSTRAIN_MOD_BUILD_BOOT_XAKEDEPENDENCYRESOLVER_HPP */
