#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETBINDERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETBINDERROR_HPP

#include "DatagramSocket.hpp"
#include "SocketBindError.hpp"
#include "DatagramSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API DatagramSocketBindError : public SocketBindError, public DatagramSocketError {

	  private:
		const DatagramSocket::ErrorCode code;

	  public:
		DatagramSocketBindError(DatagramSocket::ErrorCode);
		DatagramSocketBindError(const DatagramSocketBindError&);

		inline DatagramSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(DatagramSocketBindError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETBINDERROR_HPP */
