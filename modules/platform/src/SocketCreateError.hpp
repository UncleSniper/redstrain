#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETCREATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETCREATEERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketCreateError : public virtual SocketError {

	  public:
		SocketCreateError();
		SocketCreateError(const SocketCreateError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketCreateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETCREATEERROR_HPP */
