#include "NoSuchGoalError.hpp"

using std::string;

namespace redengine {
namespace build {

	NoSuchGoalError::NoSuchGoalError(const string& goalName) : goalName(goalName) {}

	NoSuchGoalError::NoSuchGoalError(const NoSuchGoalError& error) : BuildError(error), goalName(error.goalName) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchGoalError) {
		out << "No such goal: " << goalName;
	}

}}
