#include "DatagramSocketBindError.hpp"

namespace redengine {
namespace platform {

	DatagramSocketBindError::DatagramSocketBindError(DatagramSocket::ErrorCode code) : code(code) {}

	DatagramSocketBindError::DatagramSocketBindError(const DatagramSocketBindError& error)
			: IOError(error), SocketError(error), SocketBindError(error), DatagramSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(DatagramSocketBindError) {
		out << "Failed to bind datagram socket: " << DatagramSocket::getErrorMessage(code);
	}

}}
