#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETCLOSEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETCLOSEERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketCloseError : public virtual SocketError {

	  public:
		SocketCloseError();
		SocketCloseError(const SocketCloseError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketCloseError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETCLOSEERROR_HPP */
