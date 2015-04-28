#ifndef REDSTRAIN_MOD_PLATFORM_NETWORKSOCKETERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_NETWORKSOCKETERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API NetworkSocketError : public virtual SocketError {

	  public:
		NetworkSocketError();
		NetworkSocketError(const NetworkSocketError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(NetworkSocketError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_NETWORKSOCKETERROR_HPP */
