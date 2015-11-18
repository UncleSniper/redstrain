#include "UnrecognizedArchitectureNameError.hpp"

using std::string;

namespace redengine {
namespace build {

	UnrecognizedArchitectureNameError::UnrecognizedArchitectureNameError(const string& architecture)
			: architecture(architecture) {}

	UnrecognizedArchitectureNameError::UnrecognizedArchitectureNameError(const
			UnrecognizedArchitectureNameError& error)
			: Error(error), IllegalArgumentError(error), architecture(error.architecture) {}

	UnrecognizedArchitectureNameError::~UnrecognizedArchitectureNameError() {}

	REDSTRAIN_DEFINE_ERROR(UnrecognizedArchitectureNameError) {
		out << "Unrecognized architecture: " << architecture;
	}

}}
