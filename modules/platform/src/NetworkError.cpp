#include "NetworkError.hpp"

namespace redengine {
namespace platform {

	NetworkError::NetworkError() {}

	NetworkError::NetworkError(const NetworkError& error) : Error(error), IOError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(NetworkError)

}}
