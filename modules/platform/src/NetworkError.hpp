#ifndef REDSTRAIN_MOD_PLATFORM_NETWORKERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_NETWORKERROR_HPP

#include <redstrain/error/IOError.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API NetworkError : public virtual error::IOError {

	  public:
		NetworkError();
		NetworkError(const NetworkError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(NetworkError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_NETWORKERROR_HPP */
