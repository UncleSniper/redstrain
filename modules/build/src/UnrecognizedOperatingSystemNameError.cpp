#include "UnrecognizedOperatingSystemNameError.hpp"

using std::string;

namespace redengine {
namespace build {

	UnrecognizedOperatingSystemNameError::UnrecognizedOperatingSystemNameError(const string& architecture)
			: architecture(architecture) {}

	UnrecognizedOperatingSystemNameError::UnrecognizedOperatingSystemNameError(const
			UnrecognizedOperatingSystemNameError& error)
			: Error(error), IllegalArgumentError(error), architecture(error.architecture) {}

	UnrecognizedOperatingSystemNameError::~UnrecognizedOperatingSystemNameError() {}

	REDSTRAIN_DEFINE_ERROR(UnrecognizedOperatingSystemNameError) {
		out << "Unrecognized operating system: " << architecture;
	}

}}
