#include "TCPSocketCloseError.hpp"

namespace redengine {
namespace platform {

	TCPSocketCloseError::TCPSocketCloseError(TCPSocket::ErrorCode code) : code(code) {}

	TCPSocketCloseError::TCPSocketCloseError(const TCPSocketCloseError& error)
			: IOError(error), SocketError(error), SocketCloseError(error), TCPSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(TCPSocketCloseError) {
		out << "Failed to close TCP socket: " << TCPSocket::getErrorMessage(code);
	}

}}
