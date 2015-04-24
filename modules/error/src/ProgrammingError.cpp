#include "ProgrammingError.hpp"

using std::string;

namespace redengine {
namespace error {

	ProgrammingError::ProgrammingError(const string& message) : message(message) {}

	ProgrammingError::ProgrammingError(const ProgrammingError& error) : Error(error), message(error.message) {}

	REDSTRAIN_DEFINE_ERROR(ProgrammingError) {
		out << "Programmer fsck(8)ed up: " << message;
	}

}}
