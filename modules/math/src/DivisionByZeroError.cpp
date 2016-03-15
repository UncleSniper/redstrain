#include "DivisionByZeroError.hpp"

namespace redengine {
namespace math {

	DivisionByZeroError::DivisionByZeroError() {}

	DivisionByZeroError::DivisionByZeroError(const DivisionByZeroError& error) : MathError(error) {}

	REDSTRAIN_DEFINE_ERROR(DivisionByZeroError) {
		out << "Division by zero";
	}

}}
