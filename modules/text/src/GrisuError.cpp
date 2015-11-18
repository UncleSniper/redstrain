#include "GrisuError.hpp"

using std::string;

namespace redengine {
namespace text {

	GrisuError::GrisuError(const string& message) : ProgrammingError(message) {}

	GrisuError::GrisuError(const GrisuError& error) : ProgrammingError(error) {}

	REDSTRAIN_DEFINE_ERROR(GrisuError) {
		out << "Error in Grisu implementation: " << getRawMessage();
	}

}}
