#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETPACKETSIZEEXCEEDEDERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETPACKETSIZEEXCEEDEDERROR_HPP

#include "DatagramSocket.hpp"
#include "DatagramSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketPacketSizeExceededError : public DatagramSocketError {

	  private:
		const DatagramSocket::ErrorCode code;

	  public:
		SocketPacketSizeExceededError(DatagramSocket::ErrorCode);
		SocketPacketSizeExceededError(const SocketPacketSizeExceededError&);

		inline DatagramSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(SocketPacketSizeExceededError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETPACKETSIZEEXCEEDEDERROR_HPP */
