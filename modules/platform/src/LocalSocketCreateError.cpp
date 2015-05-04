#include "LocalSocketCreateError.hpp"

namespace redengine {
namespace platform {

	LocalSocketCreateError::LocalSocketCreateError(LocalSocket::ErrorCode code) : code(code) {}

	LocalSocketCreateError::LocalSocketCreateError(const LocalSocketCreateError& error)
			: IOError(error), SocketError(error), SocketCreateError(error), LocalSocketError(error),
			code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(LocalSocketCreateError) {
		out << "Failed to create local socket: " << LocalSocket::getErrorMessage(code);
	}

}}
