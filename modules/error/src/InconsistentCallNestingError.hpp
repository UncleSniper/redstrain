#ifndef REDSTRAIN_MOD_ERROR_INCONSISTENTCALLNESTINGERROR_HPP
#define REDSTRAIN_MOD_ERROR_INCONSISTENTCALLNESTINGERROR_HPP

#include "StateError.hpp"

namespace redengine {
namespace error {

	class REDSTRAIN_ERROR_API InconsistentCallNestingError : public StateError {

	  public:
		InconsistentCallNestingError();
		InconsistentCallNestingError(const InconsistentCallNestingError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(InconsistentCallNestingError)

	};

}}

#endif /* REDSTRAIN_MOD_ERROR_INCONSISTENTCALLNESTINGERROR_HPP */
