#ifndef REDSTRAIN_MOD_MATH_MATHERROR_HPP
#define REDSTRAIN_MOD_MATH_MATHERROR_HPP

#include <redstrain/error/Error.hpp>

#include "api.hpp"

namespace redengine {
namespace math {

	class REDSTRAIN_MATH_API MathError : public error::Error {

	  public:
		MathError();
		MathError(const MathError&);

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(MathError)

	};

}}

#endif /* REDSTRAIN_MOD_MATH_MATHERROR_HPP */
