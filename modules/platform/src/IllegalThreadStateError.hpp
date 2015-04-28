#ifndef REDSTRAIN_MOD_PLATFORM_ILLEGALTHREADSTATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_ILLEGALTHREADSTATEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "ThreadOperationError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API IllegalThreadStateError : public ThreadError, public error::StateError {

	  private:
		const ThreadOperationError::Operation operation;
		const Thread::State state;

	  public:
		IllegalThreadStateError(ThreadOperationError::Operation, Thread::State);
		IllegalThreadStateError(const IllegalThreadStateError&);

		inline ThreadOperationError::Operation getOperation() const {
			return operation;
		}

		inline Thread::State getThreadState() const {
			return state;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalThreadStateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_ILLEGALTHREADSTATEERROR_HPP */
