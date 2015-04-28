#ifndef REDSTRAIN_MOD_PLATFORM_SOCKETERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_SOCKETERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API SocketError : public virtual error::IOError {

	  public:
		SocketError();
		SocketError(const SocketError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(SocketError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_SOCKETERROR_HPP */
