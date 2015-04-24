#include "IllegalArgumentError.hpp"

using std::string;

namespace redengine {
namespace error {

	IllegalArgumentError::IllegalArgumentError(const string& message) : message(message) {}

	IllegalArgumentError::IllegalArgumentError(const IllegalArgumentError& error)
			: Error(error), message(error.message) {}

	REDSTRAIN_DEFINE_ERROR(IllegalArgumentError) {
		out << "Illegal argument: " << message;
	}

}}
