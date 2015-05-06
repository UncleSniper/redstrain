#include "TCPSocketListenError.hpp"

namespace redengine {
namespace platform {

	TCPSocketListenError::TCPSocketListenError(TCPSocket::ErrorCode code) : code(code) {}

	TCPSocketListenError::TCPSocketListenError(const TCPSocketListenError& error)
			: Error(error), IOError(error), SocketError(error), SocketListenError(error), TCPSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(TCPSocketListenError) {
		out << "Failed to listen on TCP socket: " << TCPSocket::getErrorMessage(code);
	}

}}
