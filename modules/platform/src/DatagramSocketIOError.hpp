#ifndef REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETIOERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETIOERROR_HPP

#include "NetworkError.hpp"
#include "SocketIOError.hpp"
#include "DatagramSocket.hpp"
#include "DatagramSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API DatagramSocketIOError
			: public SocketIOError, public DatagramSocketError, public NetworkError {

	  private:
		const DatagramSocket::ErrorCode code;

	  public:
		DatagramSocketIOError(Direction, DatagramSocket::ErrorCode);
		DatagramSocketIOError(const DatagramSocketIOError&);

		inline DatagramSocket::ErrorCode getErrorCode() const {
			return code;
		}

		REDSTRAIN_DECLARE_ERROR(DatagramSocketIOError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DATAGRAMSOCKETIOERROR_HPP */
