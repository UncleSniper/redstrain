#include "NoSuchValveError.hpp"

using std::string;

namespace redengine {
namespace build {

	NoSuchValveError::NoSuchValveError(const string& valve) : valve(valve) {}

	NoSuchValveError::NoSuchValveError(const NoSuchValveError& error) : BuildError(error), valve(error.valve) {}

	REDSTRAIN_DEFINE_ERROR(NoSuchValveError) {
		out << "No such valve: " << valve;
	}

}}
