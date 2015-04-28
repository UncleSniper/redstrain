#include "MutexOperationError.hpp"

namespace redengine {
namespace platform {

	MutexOperationError::MutexOperationError(Operation operation, Mutex::ErrorCode code)
			: operation(operation), code(code) {}

	MutexOperationError::MutexOperationError(const MutexOperationError& error)
			: Error(error), MutexError(error), operation(error.operation), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(MutexOperationError) {
		out << "Failed to ";
		switch(operation) {
			case CREATE:
				out << "create mutex";
				break;
			case DESTROY:
				out << "destroy mutex";
				break;
			case LOCK:
				out << "lock mutex";
				break;
			case UNLOCK:
				out << "unlock mutex";
				break;
			default:
				out << "perform requested operation on mutex";
				break;
		}
		out << ": " << Mutex::getErrorMessage(code);
	}

}}
