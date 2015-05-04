#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETLISTENERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETLISTENERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketListenError : public virtual SocketError {

	  public:
		SocketListenError();
		SocketListenError(const SocketListenError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketListenError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETLISTENERROR_HPP */
