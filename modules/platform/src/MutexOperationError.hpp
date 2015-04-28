#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXOPERATIONERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXOPERATIONERROR_HPP

#include "Mutex.hpp"
#include "MutexError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API MutexOperationError : public MutexError {

	  public:
		enum Operation {
			CREATE,
			DESTROY,
			LOCK,
			UNLOCK
		};

	  private:
		const Operation operation;
		const Mutex::ErrorCode code;

	  public:
		MutexOperationError(Operation, Mutex::ErrorCode);
		MutexOperationError(const MutexOperationError&);

		inline Operation getOperation() const {
			return operation;
		}

		inline Mutex::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(MutexOperationError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXOPERATIONERROR_HPP */
