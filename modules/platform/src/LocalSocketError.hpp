#ifndef REDSTRAIN_MOD_PLATFORM_LOCALSOCKETERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_LOCALSOCKETERROR_HPP

#include "SocketError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API LocalSocketError : public virtual SocketError {

	  public:
		LocalSocketError();
		LocalSocketError(const LocalSocketError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(LocalSocketError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_LOCALSOCKETERROR_HPP */
