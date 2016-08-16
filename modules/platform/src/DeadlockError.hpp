#ifndef REDSTRAIN_MOD_PLATFORM_DEADLOCKERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_DEADLOCKERROR_HPP

#include "ThreadingError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API DeadlockError : public ThreadingError {

	  public:
		DeadlockError();
		DeadlockError(const DeadlockError&);

		REDSTRAIN_DECLARE_ERROR(DeadlockError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_DEADLOCKERROR_HPP */
