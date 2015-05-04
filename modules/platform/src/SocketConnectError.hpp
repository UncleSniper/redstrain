#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETCONNECTERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETCONNECTERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketConnectError : public virtual SocketError {

	  public:
		SocketConnectError();
		SocketConnectError(const SocketConnectError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketConnectError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETCONNECTERROR_HPP */
