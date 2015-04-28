#ifndef REDSTRAIN_MOD_PLATFORM_THREADINGERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_THREADINGERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ThreadingError : public virtual error::Error {

	  public:
		ThreadingError();
		ThreadingError(const ThreadingError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(ThreadingError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_THREADINGERROR_HPP */
