#include "IllegalMutexStateError.hpp"

namespace redengine {
namespace platform {

	IllegalMutexStateError::IllegalMutexStateError(MutexOperationError::Operation operation)
			: operation(operation) {}

	IllegalMutexStateError::IllegalMutexStateError(const IllegalMutexStateError& error)
			: Error(error), MutexError(error), StateError(error), operation(error.operation) {}

	REDSTRAIN_DEFINE_ERROR(IllegalMutexStateError) {
		out << "Cannot ";
		switch(operation) {
			case MutexOperationError::CREATE:
				out << "create mutex";
				break;
			case MutexOperationError::DESTROY:
				out << "destroy mutex";
				break;
			case MutexOperationError::LOCK:
				out << "lock mutex";
				break;
			case MutexOperationError::UNLOCK:
				out << "unlock mutex";
				break;
			default:
				out << "perform requested operation on mutex";
				break;
		}
		out << ", as it has already been destroyed";
	}

}}
