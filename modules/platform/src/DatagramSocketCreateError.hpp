#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETCREATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETCREATEERROR_HPP

#include "DatagramSocket.hpp"
#include "SocketCreateError.hpp"
#include "DatagramSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API DatagramSocketCreateError
			: public SocketCreateError, public DatagramSocketError {

	  private:
		const DatagramSocket::ErrorCode code;

	  public:
		DatagramSocketCreateError(DatagramSocket::ErrorCode);
		DatagramSocketCreateError(const DatagramSocketCreateError&);

		inline DatagramSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(DatagramSocketCreateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETCREATEERROR_HPP */
