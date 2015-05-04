#ifndef REDSTRAIN_MOD_PLATFORM_TCPSOCKETERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_TCPSOCKETERROR_HPP

#include "NetworkSocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API TCPSocketError : public NetworkSocketError {

	  public:
		TCPSocketError();
		TCPSocketError(const TCPSocketError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(TCPSocketError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_TCPSOCKETERROR_HPP */
