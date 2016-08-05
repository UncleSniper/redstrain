#ifndef REDSTRAIN_MOD_ALGORITHM_NOELEMENTINTHISSTATEERROR_HPP
#define REDSTRAIN_MOD_ALGORITHM_NOELEMENTINTHISSTATEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "api.hpp"

namespace redengine {
namespace algorithm {

	class REDSTRAIN_ALGORITHM_API NoElementInThisStateError : public error::StateError {

	  public:
		NoElementInThisStateError();
		NoElementInThisStateError(const NoElementInThisStateError&);

		REDSTRAIN_DECLARE_ERROR(NoElementInThisStateError)

	};

}}

#endif /* REDSTRAIN_MOD_ALGORITHM_NOELEMENTINTHISSTATEERROR_HPP */
