#ifndef REDSTRAIN_MOD_PLATFORM_INCONSISTENTMUTEXUNLOCKERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_INCONSISTENTMUTEXUNLOCKERROR_HPP

#include <redstrain/error/InconsistentCallNestingError.hpp>

#include "MutexError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API InconsistentMutexUnlockError
			: public MutexError, public error::InconsistentCallNestingError {

	  public:
		InconsistentMutexUnlockError();
		InconsistentMutexUnlockError(const InconsistentMutexUnlockError&);

		REDSTRAIN_DECLARE_ERROR(InconsistentMutexUnlockError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_INCONSISTENTMUTEXUNLOCKERROR_HPP */
