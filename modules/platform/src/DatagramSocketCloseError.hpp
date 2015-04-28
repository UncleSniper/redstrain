#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETCLOSEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETCLOSEERROR_HPP

#include "DatagramSocket.hpp"
#include "SocketCloseError.hpp"
#include "DatagramSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API DatagramSocketCloseError : public SocketCloseError, public DatagramSocketError {

	  private:
		const DatagramSocket::ErrorCode code;

	  public:
		DatagramSocketCloseError(DatagramSocket::ErrorCode);
		DatagramSocketCloseError(const DatagramSocketCloseError&);

		inline DatagramSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(DatagramSocketCloseError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETCLOSEERROR_HPP */
