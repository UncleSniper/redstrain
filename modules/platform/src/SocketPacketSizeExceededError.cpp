#include "SocketPacketSizeExceededError.hpp"

namespace redengine {
namespace platform {

	SocketPacketSizeExceededError::SocketPacketSizeExceededError(DatagramSocket::ErrorCode code) : code(code) {}

	SocketPacketSizeExceededError::SocketPacketSizeExceededError(const SocketPacketSizeExceededError& error)
			: Error(error), IOError(error), SocketError(error), DatagramSocketError(error), code(error.code) {}

	REDSTRAIN_DEFINE_ERROR(SocketPacketSizeExceededError) {
		out << "Packed size for underlying socket protocol exceeded";
	}

}}
