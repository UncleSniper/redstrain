#include "LocalSocketIOError.hpp"

namespace redengine {
namespace platform {

	LocalSocketIOError::LocalSocketIOError(Direction dir, LocalSocket::ErrorCode code)
			: SocketIOError(dir), code(code) {}

	LocalSocketIOError::LocalSocketIOError(const LocalSocketIOError& error)
			: IOError(error), SocketError(error), SocketIOError(error), LocalSocketError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(LocalSocketIOError) {
		printDirection(out);
		out << "across local socket: " << LocalSocket::getErrorMessage(code);
	}

}}
