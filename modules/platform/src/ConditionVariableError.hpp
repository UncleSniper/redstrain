#ifndef REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLEERROR_HPP
#define REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLEERROR_HPP

#include "ThreadingError.hpp"

namespace redengine {
namespace platform {

	class REDSTRAIN_PLATFORM_API ConditionVariableError : public ThreadingError {

	  public:
		ConditionVariableError();
		ConditionVariableError(const ConditionVariableError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(ConditionVariableError)

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_CONDITIONVARIABLEERROR_HPP */
