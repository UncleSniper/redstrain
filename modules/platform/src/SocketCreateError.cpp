#include "SocketCreateError.hpp"

namespace redengine {
namespace platform {

	SocketCreateError::SocketCreateError() {}

	SocketCreateError::SocketCreateError(const SocketCreateError& error)
			: Error(error), IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketCreateError)

}}
