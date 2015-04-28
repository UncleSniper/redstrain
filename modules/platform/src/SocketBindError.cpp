#include "SocketBindError.hpp"

namespace redengine {
namespace platform {

	SocketBindError::SocketBindError() {}

	SocketBindError::SocketBindError(const SocketBindError& error) : IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketBindError)

}}
