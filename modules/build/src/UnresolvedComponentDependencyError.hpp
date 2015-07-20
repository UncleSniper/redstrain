#ifndef REDSTRAIN_MOD_BUILD_UNRESOLVEDCOMPONENTDEPENDENCYERROR_HPP
#define REDSTRAIN_MOD_BUILD_UNRESOLVEDCOMPONENTDEPENDENCYERROR_HPP

#include "NoSuchComponentError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API UnresolvedComponentDependencyError : public NoSuchComponentError {

	  private:
		const std::string outer;

	  public:
		UnresolvedComponentDependencyError(const std::string&, const std::string&);
		UnresolvedComponentDependencyError(const UnresolvedComponentDependencyError&);

		inline const std::string& getDependingComponentName() const {
			return outer;
		}

		REDSTRAIN_DECLARE_ERROR(UnresolvedComponentDependencyError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_UNRESOLVEDCOMPONENTDEPENDENCYERROR_HPP */
