#include "ThreadOperationError.hpp"

namespace redengine {
namespace platform {

	ThreadOperationError::ThreadOperationError(Operation operation, Thread::ErrorCode code)
			: operation(operation), code(code) {}

	ThreadOperationError::ThreadOperationError(const ThreadOperationError& error)
			: Error(error), ThreadError(error), operation(error.operation), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(ThreadOperationError) {
		out << "Failed to ";
		switch(operation) {
			case ThreadOperationError::START:
				out << "start thread";
				break;
			case ThreadOperationError::STOP:
				out << "stop thread";
				break;
			case ThreadOperationError::INTERRUPT:
				out << "interrupt thread";
				break;
			case ThreadOperationError::JOIN:
				out << "join thread";
				break;
			default:
				out << "perform requested operation on thread";
				break;
		}
		out << ": " << Thread::getErrorMessage(code);
	}

}}
