#include "IllegalThreadStateError.hpp"

namespace redengine {
namespace platform {

	IllegalThreadStateError::IllegalThreadStateError(ThreadOperationError::Operation operation, Thread::State state)
			: operation(operation), state(state) {}

	IllegalThreadStateError::IllegalThreadStateError(const IllegalThreadStateError& error)
			: Error(error), ThreadError(error), StateError(error), operation(error.operation), state(error.state) {}

	REDSTRAIN_DEFINE_ERROR(IllegalThreadStateError) {
		out << "Cannot ";
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
		out << ", as it is ";
		switch(state) {
			case Thread::PRISTINE:
				out << "pristine";
				break;
			case Thread::JOINABLE:
				out << "joinable";
				break;
			case Thread::DETACHED:
				out << "detached";
				break;
			case Thread::TERMINATED:
				out << "terminated";
				break;
			default:
				out << "in unknown state";
				break;
		}
	}

}}
