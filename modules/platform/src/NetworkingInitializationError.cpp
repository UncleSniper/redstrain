#include "NetworkingInitializationError.hpp"

namespace redengine {
namespace platform {

	NetworkingInitializationError::NetworkingInitializationError(Networking::ErrorCode code) : code(code) {}

	NetworkingInitializationError::NetworkingInitializationError(const NetworkingInitializationError& error)
			: OSBindError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(NetworkingInitializationError) {
		out << "Failed to initialize networking subsystem: " << Networking::getErrorMessage(code);
	}

}}
