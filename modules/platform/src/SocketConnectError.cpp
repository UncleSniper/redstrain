#include "SocketConnectError.hpp"

namespace redengine {
namespace platform {

	SocketConnectError::SocketConnectError() {}

	SocketConnectError::SocketConnectError(const SocketConnectError& error) : IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketConnectError)

}}
