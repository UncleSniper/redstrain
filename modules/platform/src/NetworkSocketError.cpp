#include "NetworkSocketError.hpp"

namespace redengine {
namespace platform {

	NetworkSocketError::NetworkSocketError() {}

	NetworkSocketError::NetworkSocketError(const NetworkSocketError& error) : IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(NetworkSocketError)

}}
