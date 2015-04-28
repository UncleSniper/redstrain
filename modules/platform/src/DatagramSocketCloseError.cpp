#include "DatagramSocketCloseError.hpp"

namespace redengine {
namespace platform {

	DatagramSocketCloseError::DatagramSocketCloseError(DatagramSocket::ErrorCode code) : code(code) {}

	DatagramSocketCloseError::DatagramSocketCloseError(const DatagramSocketCloseError& error)
			: IOError(error), SocketError(error), SocketCloseError(error), DatagramSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(DatagramSocketCloseError) {
		out << "Failed to close datagram socket: " << DatagramSocket::getErrorMessage(code);
	}

}}
