#include "LocalSocketCloseError.hpp"

namespace redengine {
namespace platform {

	LocalSocketCloseError::LocalSocketCloseError(LocalSocket::ErrorCode code) : code(code) {}

	LocalSocketCloseError::LocalSocketCloseError(const LocalSocketCloseError& error)
			: IOError(error), SocketError(error), SocketCloseError(error), LocalSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(LocalSocketCloseError) {
		out << "Failed to close local socket: " << LocalSocket::getErrorMessage(code);
	}

}}
