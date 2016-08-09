#include "ParameterizedStringStackOverflowError.hpp"

namespace redengine {
namespace damnation {

	ParameterizedStringStackOverflowError::ParameterizedStringStackOverflowError() {}

	ParameterizedStringStackOverflowError::ParameterizedStringStackOverflowError(const
			ParameterizedStringStackOverflowError& error) : Error(error), TParmError(error) {}

	REDSTRAIN_DEFINE_ERROR(ParameterizedStringStackOverflowError) {
		out << "Parameterized string stack overflow";
	}

}}
