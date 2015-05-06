#include "TCPSocketConnectError.hpp"

namespace redengine {
namespace platform {

	TCPSocketConnectError::TCPSocketConnectError(TCPSocket::ErrorCode code) : code(code) {}

	TCPSocketConnectError::TCPSocketConnectError(const TCPSocketConnectError& error)
			: Error(error), IOError(error), SocketError(error), SocketConnectError(error), TCPSocketError(error),
			NetworkError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(TCPSocketConnectError) {
		out << "Failed to connect TCP socket: " << TCPSocket::getErrorMessage(code);
	}

}}
