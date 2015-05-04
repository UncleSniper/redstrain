#include "SocketAcceptError.hpp"

namespace redengine {
namespace platform {

	SocketAcceptError::SocketAcceptError() {}

	SocketAcceptError::SocketAcceptError(const SocketAcceptError& error) : IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(SocketAcceptError)

}}
