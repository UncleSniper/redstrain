#include "SocketCloseError.hpp"

namespace redengine {
namespace platform {

	SocketCloseError::SocketCloseError() {}

	SocketCloseError::SocketCloseError(const SocketCloseError& error) : IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketCloseError)

}}
