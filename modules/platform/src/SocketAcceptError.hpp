#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETACCEPTERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETACCEPTERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketAcceptError : public virtual SocketError {

	  public:
		SocketAcceptError();
		SocketAcceptError(const SocketAcceptError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketAcceptError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETACCEPTERROR_HPP */
