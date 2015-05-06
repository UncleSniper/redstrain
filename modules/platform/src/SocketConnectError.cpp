#include "SocketConnectError.hpp"

namespace redengine {
namespace platform {

	SocketConnectError::SocketConnectError() {}

	SocketConnectError::SocketConnectError(const SocketConnectError& error)
			: Error(error), IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketConnectError)

}}
