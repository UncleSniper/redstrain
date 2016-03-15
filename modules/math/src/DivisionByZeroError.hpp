#ifndef REDSTRAIN_MOD_MATH_DIVISIONBYZEROERROR_HPP
#define REDSTRAIN_MOD_MATH_DIVISIONBYZEROERROR_HPP

#include "MathError.hpp"

namespace redengine {
namespace math {

	class REDSTRAIN_MATH_API DivisionByZeroError : public MathError {

	  public:
		DivisionByZeroError();
		DivisionByZeroError(const DivisionByZeroError&);

		REDSTRAIN_DECLARE_ERROR(DivisionByZeroError)

	};

}}

#endif /* REDSTRAIN_MOD_MATH_DIVISIONBYZEROERROR_HPP */
