#include "LocalSocketAcceptError.hpp"

namespace redengine {
namespace platform {

	LocalSocketAcceptError::LocalSocketAcceptError(LocalSocket::ErrorCode code) : code(code) {}

	LocalSocketAcceptError::LocalSocketAcceptError(const LocalSocketAcceptError& error)
			: IOError(error), SocketError(error), SocketAcceptError(error), LocalSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(LocalSocketAcceptError) {
		out << "Failed to accept from local socket: " << LocalSocket::getErrorMessage(code);
	}

}}
