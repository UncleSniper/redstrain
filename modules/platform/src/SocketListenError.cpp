#include "SocketListenError.hpp"

namespace redengine {
namespace platform {

	SocketListenError::SocketListenError() {}

	SocketListenError::SocketListenError(const SocketListenError& error) : IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketListenError)

}}
