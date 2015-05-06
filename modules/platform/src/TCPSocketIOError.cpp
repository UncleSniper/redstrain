#include "TCPSocketIOError.hpp"

namespace redengine {
namespace platform {

	TCPSocketIOError::TCPSocketIOError(Direction dir, TCPSocket::ErrorCode code) : SocketIOError(dir), code(code) {}

	TCPSocketIOError::TCPSocketIOError(const TCPSocketIOError& error)
			: Error(error), IOError(error), SocketError(error), SocketIOError(error), TCPSocketError(error),
			NetworkError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(TCPSocketIOError) {
		printDirection(out);
		out << "across TCP socket: " << TCPSocket::getErrorMessage(code);
	}

}}
