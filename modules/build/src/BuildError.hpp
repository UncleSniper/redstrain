#ifndef REDSTRAIN_MOD_BUILD_BUILDERROR_HPP
#define REDSTRAIN_MOD_BUILD_BUILDERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API BuildError : public error::Error {

	  public:
		BuildError();
		BuildError(const BuildError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(BuildError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_BUILDERROR_HPP */
