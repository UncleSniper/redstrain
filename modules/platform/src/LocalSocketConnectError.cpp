#include "LocalSocketConnectError.hpp"

namespace redengine {
namespace platform {

	LocalSocketConnectError::LocalSocketConnectError(LocalSocket::ErrorCode code) : code(code) {}

	LocalSocketConnectError::LocalSocketConnectError(const LocalSocketConnectError& error)
			: Error(error), IOError(error), SocketError(error), SocketConnectError(error), LocalSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(LocalSocketConnectError) {
		out << "Failed to connect local socket: " << LocalSocket::getErrorMessage(code);
	}

}}
