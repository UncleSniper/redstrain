#include "ProcessOperationError.hpp"

namespace redengine {
namespace platform {

	ProcessOperationError::ProcessOperationError(Operation operation, Process::ErrorCode code)
			: operation(operation), code(code) {}

	ProcessOperationError::ProcessOperationError(const ProcessOperationError& error)
			: Error(error), ProcessError(error), operation(error.operation), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(ProcessOperationError) {
		out << "Failed to ";
		switch(operation) {
			case ProcessOperationError::START:
				out << "start process";
				break;
			case ProcessOperationError::TERMINATE:
				out << "terminate process";
				break;
			case ProcessOperationError::KILL:
				out << "kill process";
				break;
			case ProcessOperationError::INTERRUPT:
				out << "interrupt process";
				break;
			case ProcessOperationError::WAIT:
				out << "wait for process";
				break;
			default:
				out << "perform requested operation on process";
				break;
		}
		out << ": " << Process::getErrorMessage(code);
	}

}}
