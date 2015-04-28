#ifndef REDSTRAIN_MOD_PLATFORM_MUTEXERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_MUTEXERROR_HPP

#include "ThreadingError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API MutexError : public ThreadingError {

	  public:
		MutexError();
		MutexError(const MutexError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(MutexError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_MUTEXERROR_HPP */
