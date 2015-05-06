#include "DatagramSocketError.hpp"

namespace redengine {
namespace platform {

	DatagramSocketError::DatagramSocketError() {}

	DatagramSocketError::DatagramSocketError(const DatagramSocketError& error)
			: Error(error), IOError(error), SocketError(error), NetworkSocketError(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(DatagramSocketError)

}}
