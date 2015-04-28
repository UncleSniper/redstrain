#ifndef REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLEOPERATIONERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLEOPERATIONERROR_HPP

#include "ConditionVariable.hpp"
#include "ConditionVariableError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ConditionVariableOperationError : public ConditionVariableError {

	  public:
		enum Operation {
			CREATE,
			DESTROY,
			WAIT,
			SIGNAL
		};

	  private:
		const Operation operation;
		const ConditionVariable::ErrorCode code;

	  public:
		ConditionVariableOperationError(Operation, ConditionVariable::ErrorCode);
		ConditionVariableOperationError(const ConditionVariableOperationError&);

		inline Operation getOperation() const {
			return operation;
		}

		inline ConditionVariable::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(ConditionVariableOperationError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLEOPERATIONERROR_HPP */
