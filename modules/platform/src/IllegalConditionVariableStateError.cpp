#include "IllegalConditionVariableStateError.hpp"

namespace redengine {
namespace platform {

	IllegalConditionVariableStateError::IllegalConditionVariableStateError(ConditionVariableOperationError::Operation
			operation) : operation(operation) {}

	IllegalConditionVariableStateError::IllegalConditionVariableStateError(const IllegalConditionVariableStateError&
			error) : Error(error), ConditionVariableError(error), StateError(error), operation(error.operation) {}

	REDSTRAIN_DEFINE_ERROR(IllegalConditionVariableStateError) {
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
		out << ", as it has already been destroyed";
	}

}}
