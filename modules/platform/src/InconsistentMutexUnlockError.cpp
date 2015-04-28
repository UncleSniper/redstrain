#include "InconsistentMutexUnlockError.hpp"

namespace redengine {
namespace platform {

	InconsistentMutexUnlockError::InconsistentMutexUnlockError() {}

	InconsistentMutexUnlockError::InconsistentMutexUnlockError(const InconsistentMutexUnlockError& error)
			: Error(error), MutexError(error), InconsistentCallNestingError(error) {}

	REDSTRAIN_DEFINE_ERROR(InconsistentMutexUnlockError) {
		out << "Cannot unlock mutex, as it is not owned by the calling thread";
	}

}}
