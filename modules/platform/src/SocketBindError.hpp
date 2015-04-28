#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETBINDERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETBINDERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketBindError : public virtual SocketError {

	  public:
		SocketBindError();
		SocketBindError(const SocketBindError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketBindError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETBINDERROR_HPP */
