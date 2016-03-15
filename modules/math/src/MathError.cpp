#include "MathError.hpp"

namespace redengine {
namespace math {

	MathError::MathError() {}

	MathError::MathError(const MathError& error) : Error(error) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(MathError)

}}
