#include "TCPSocketCreateError.hpp"

namespace redengine {
namespace platform {

	TCPSocketCreateError::TCPSocketCreateError(TCPSocket::ErrorCode code) : code(code) {}

	TCPSocketCreateError::TCPSocketCreateError(const TCPSocketCreateError& error)
			: Error(error), IOError(error), SocketError(error), SocketCreateError(error), TCPSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(TCPSocketCreateError) {
		out << "Failed to create TCP socket: " << TCPSocket::getErrorMessage(code);
	}

}}
