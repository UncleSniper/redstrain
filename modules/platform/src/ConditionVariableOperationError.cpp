#include "ConditionVariableOperationError.hpp"

namespace redengine {
namespace platform {

	ConditionVariableOperationError::ConditionVariableOperationError(Operation operation,
			ConditionVariable::ErrorCode code) : operation(operation), code(code) {}

	ConditionVariableOperationError::ConditionVariableOperationError(const ConditionVariableOperationError& error)
			: Error(error), ConditionVariableError(error), operation(error.operation), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(ConditionVariableOperationError) {
		out << "Failed to ";
		switch(operation) {
			case CREATE:
				out << "create condition variable";
				break;
			case DESTROY:
				out << "destroy condition variable";
				break;
			case WAIT:
				out << "wait on condition variable";
				break;
			case SIGNAL:
				out << "signal condition variable";
				break;
			default:
				out << "perform requested operation on condition variable";
				break;
		}
		out << ": " << ConditionVariable::getErrorMessage(code);
	}

}}
