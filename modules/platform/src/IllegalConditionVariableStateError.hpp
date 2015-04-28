#ifndef REDSTRAIN_MOD_PLATFORM_ILLEGALCONDITIONVARIABLESTATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_ILLEGALCONDITIONVARIABLESTATEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "ConditionVariableOperationError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API IllegalConditionVariableStateError
			: public ConditionVariableError, public error::StateError {

	  private:
		const ConditionVariableOperationError::Operation operation;

	  public:
		IllegalConditionVariableStateError(ConditionVariableOperationError::Operation);
		IllegalConditionVariableStateError(const IllegalConditionVariableStateError&);

		inline ConditionVariableOperationError::Operation getOperation() const {
			return operation;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalConditionVariableStateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_ILLEGALCONDITIONVARIABLESTATEERROR_HPP */
