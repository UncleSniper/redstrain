#ifndef REDSTRAIN_MOD_PLATFORM_THREADOPERATIONERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_THREADOPERATIONERROR_HPP

#include "Thread.hpp"
#include "ThreadError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ThreadOperationError : public ThreadError {

	  public:
		enum Operation {
			START,
			STOP,
			INTERRUPT,
			JOIN,
			SETUP_UNWINDER
		};

	  private:
		const Operation operation;
		const Thread::ErrorCode code;

	  public:
		ThreadOperationError(Operation, Thread::ErrorCode);
		ThreadOperationError(const ThreadOperationError&);

		inline Operation getOperation() const {
			return operation;
		}

		inline Thread::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(ThreadOperationError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_THREADOPERATIONERROR_HPP */
