#include "TCPSocketAcceptError.hpp"

namespace redengine {
namespace platform {

	TCPSocketAcceptError::TCPSocketAcceptError(TCPSocket::ErrorCode code) : code(code) {}

	TCPSocketAcceptError::TCPSocketAcceptError(const TCPSocketAcceptError& error)
			: Error(error), IOError(error), SocketError(error), SocketAcceptError(error), TCPSocketError(error),
			NetworkError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(TCPSocketAcceptError) {
		out << "Failed to accept from TCP socket: " << TCPSocket::getErrorMessage(code);
	}

}}
