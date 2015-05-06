#include "TCPSocketError.hpp"

namespace redengine {
namespace platform {

	TCPSocketError::TCPSocketError() {}

	TCPSocketError::TCPSocketError(const TCPSocketError& error)
			: Error(error), IOError(error), SocketError(error), NetworkSocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(TCPSocketError)

}}
