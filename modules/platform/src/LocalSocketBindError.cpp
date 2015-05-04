#include "LocalSocketBindError.hpp"

namespace redengine {
namespace platform {

	LocalSocketBindError::LocalSocketBindError(LocalSocket::ErrorCode code) : code(code) {}

	LocalSocketBindError::LocalSocketBindError(const LocalSocketBindError& error)
			: IOError(error), SocketError(error), SocketBindError(error), LocalSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(LocalSocketBindError) {
		out << "Failed to bind local socket: " << LocalSocket::getErrorMessage(code);
	}

}}
