#ifndef REDSTRAIN_MOD_PLATFORM_ILLEGALCONDITIONLOCKSTATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_ILLEGALCONDITIONLOCKSTATEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "ConditionVariableOperationError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API IllegalConditionLockStateError
			: public ConditionVariableError, public error::StateError {

	  private:
		const ConditionVariableOperationError::Operation operation;

	  public:
		IllegalConditionLockStateError(ConditionVariableOperationError::Operation);
		IllegalConditionLockStateError(const IllegalConditionLockStateError&);

		inline ConditionVariableOperationError::Operation getOperation() const {
			return operation;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalConditionLockStateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_ILLEGALCONDITIONLOCKSTATEERROR_HPP */
