#ifndef REDSTRAIN_MOD_PLATFORM_ILLEGALMUTEXSTATEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_ILLEGALMUTEXSTATEERROR_HPP

#include <redstrain/error/StateError.hpp>

#include "MutexOperationError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API IllegalMutexStateError
			: public MutexError, public error::StateError {

	  private:
		const MutexOperationError::Operation operation;

	  public:
		IllegalMutexStateError(MutexOperationError::Operation);
		IllegalMutexStateError(const IllegalMutexStateError&);

		inline MutexOperationError::Operation getOperation() const {
			return operation;
		}

		REDSTRAIN_DECLARE_ERROR(IllegalMutexStateError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_ILLEGALMUTEXSTATEERROR_HPP */
