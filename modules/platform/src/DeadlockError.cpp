#include "DeadlockError.hpp"

namespace redengine {
namespace platform {

	DeadlockError::DeadlockError() {}

	DeadlockError::DeadlockError(const DeadlockError& error) : Error(error), ThreadingError(error) {}

	REDSTRAIN_DEFINE_ERROR(DeadlockError) {
		out << "Deadlock detected";
	}

}}
