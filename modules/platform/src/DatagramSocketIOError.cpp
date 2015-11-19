#include "DatagramSocketIOError.hpp"

namespace redengine {
namespace platform {

	DatagramSocketIOError::DatagramSocketIOError(Direction dir, DatagramSocket::ErrorCode code)
			: SocketIOError(dir), code(code) {}

	DatagramSocketIOError::DatagramSocketIOError(const DatagramSocketIOError& error)
			: Error(error), IOError(error), SocketError(error), SocketIOError(error), DatagramSocketError(error),
			NetworkError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(DatagramSocketIOError) {
		printDirection(out);
		out << " across datagram socket: " << DatagramSocket::getErrorMessage(code);
	}

}}
