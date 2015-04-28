#include "ConditionVariableError.hpp"

namespace redengine {
namespace platform {

	ConditionVariableError::ConditionVariableError() {}

	ConditionVariableError::ConditionVariableError(const ConditionVariableError& error)
			: Error(error), ThreadingError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(ConditionVariableError)

}}
