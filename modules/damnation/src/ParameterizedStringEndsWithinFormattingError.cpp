#include "ParameterizedStringEndsWithinFormattingError.hpp"

namespace redengine {
namespace damnation {

	ParameterizedStringEndsWithinFormattingError::ParameterizedStringEndsWithinFormattingError() {}

	ParameterizedStringEndsWithinFormattingError::ParameterizedStringEndsWithinFormattingError(const
			ParameterizedStringEndsWithinFormattingError& error) : Error(error), TParmError(error) {}

	REDSTRAIN_DEFINE_ERROR(ParameterizedStringEndsWithinFormattingError) {
		out << "Parameterized string ends within a formatting";
	}

}}
