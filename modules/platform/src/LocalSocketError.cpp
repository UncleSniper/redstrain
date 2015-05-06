#include "LocalSocketError.hpp"

namespace redengine {
namespace platform {

	LocalSocketError::LocalSocketError() {}

	LocalSocketError::LocalSocketError(const LocalSocketError& error)
			: Error(error), IOError(error), SocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(LocalSocketError)

}}
