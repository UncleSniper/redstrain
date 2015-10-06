#ifndef REDSTRAIN_MOD_BUILD_NODEFAULTGOALERROR_HPP
#define REDSTRAIN_MOD_BUILD_NODEFAULTGOALERROR_HPP

#include "BuildError.hpp"

namespace redengine {
namespace build {

	class REDSTRAIN_BUILD_API NoDefaultGoalError : public BuildError {

	  public:
		NoDefaultGoalError();
		NoDefaultGoalError(const NoDefaultGoalError&);

		REDSTRAIN_DECLARE_ERROR(NoDefaultGoalError)

	};

}}

#endif /* REDSTRAIN_MOD_BUILD_NODEFAULTGOALERROR_HPP */
