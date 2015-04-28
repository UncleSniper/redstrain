#include "IllegalConditionLockStateError.hpp"

namespace redengine {
namespace platform {

	IllegalConditionLockStateError::IllegalConditionLockStateError(ConditionVariableOperationError::Operation
			operation) : operation(operation) {}

	IllegalConditionLockStateError::IllegalConditionLockStateError(const IllegalConditionLockStateError& error)
			: Error(error), ConditionVariableError(error), StateError(error), operation(error.operation) {}

	REDSTRAIN_DEFINE_ERROR(IllegalConditionLockStateError) {
		out << "Cannot ";
		switch(operation) {
			case ConditionVariableOperationError::CREATE:
				out << "create condition variable";
				break;
			case ConditionVariableOperationError::DESTROY:
				out << "destroy condition variable";
				break;
			case ConditionVariableOperationError::WAIT:
				out << "wait on condition variable";
				break;
			case ConditionVariableOperationError::SIGNAL:
				out << "signal condition variable";
				break;
			default:
				out << "perform requested operation on condition variable";
				break;
		}
		out << ", the associated mutex is not owned by the calling thread";
	}

}}
