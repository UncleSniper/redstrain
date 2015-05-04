#include "LocalSocketListenError.hpp"

namespace redengine {
namespace platform {

	LocalSocketListenError::LocalSocketListenError(LocalSocket::ErrorCode code) : code(code) {}

	LocalSocketListenError::LocalSocketListenError(const LocalSocketListenError& error)
			: IOError(error), SocketError(error), SocketListenError(error), LocalSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(LocalSocketListenError) {
		out << "Failed to listen on local socket: " << LocalSocket::getErrorMessage(code);
	}

}}
