#include "DatagramSocketCreateError.hpp"

namespace redengine {
namespace platform {

	DatagramSocketCreateError::DatagramSocketCreateError(DatagramSocket::ErrorCode code) : code(code) {}

	DatagramSocketCreateError::DatagramSocketCreateError(const DatagramSocketCreateError& error)
			: IOError(error), SocketError(error), SocketCreateError(error), DatagramSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(DatagramSocketCreateError) {
		out << "Failed to create datagram socket: " << DatagramSocket::getErrorMessage(code);
	}

}}
