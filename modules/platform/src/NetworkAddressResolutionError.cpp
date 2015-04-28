#include "NetworkAddressResolutionError.hpp"

namespace redengine {
namespace platform {

	NetworkAddressResolutionError::NetworkAddressResolutionError(Networking::ErrorCode code) : code(code) {}

	NetworkAddressResolutionError::NetworkAddressResolutionError(const NetworkAddressResolutionError& error)
			: IOError(error), NetworkError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(NetworkAddressResolutionError) {
		out << "Failed to resolve network address: " << Networking::getErrorMessage(code);
	}

}}
