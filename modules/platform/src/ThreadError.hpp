#ifndef REDSTRAIN_MOD_PLATFORM_THREADERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_THREADERROR_HPP

#include "ThreadingError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ThreadError : public ThreadingError {

	  public:
		ThreadError();
		ThreadError(const ThreadError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(ThreadError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_THREADERROR_HPP */
