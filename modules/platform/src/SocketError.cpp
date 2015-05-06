#include "SocketError.hpp"

namespace redengine {
namespace platform {

	SocketError::SocketError() {}

	SocketError::SocketError(const SocketError& error) : Error(error), IOError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketError)

}}
