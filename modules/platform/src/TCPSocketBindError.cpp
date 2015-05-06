#include "TCPSocketBindError.hpp"

namespace redengine {
namespace platform {

	TCPSocketBindError::TCPSocketBindError(TCPSocket::ErrorCode code) : code(code) {}

	TCPSocketBindError::TCPSocketBindError(const TCPSocketBindError& error)
			: Error(error), IOError(error), SocketError(error), SocketBindError(error), TCPSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(TCPSocketBindError) {
		out << "Failed to bind TCP socket: " << TCPSocket::getErrorMessage(code);
	}

}}
