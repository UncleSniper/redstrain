#include "NoDefaultGoalError.hpp"

namespace redengine {
namespace build {

	NoDefaultGoalError::NoDefaultGoalError() {}

	NoDefaultGoalError::NoDefaultGoalError(const NoDefaultGoalError& error) : BuildError(error) {}

	REDSTRAIN_DEFINE_ERROR(NoDefaultGoalError) {
		out << "No default goal is defined";
	}

}}
