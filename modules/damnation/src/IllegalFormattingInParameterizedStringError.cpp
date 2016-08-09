#include "IllegalFormattingInParameterizedStringError.hpp"

namespace redengine {
namespace damnation {

	IllegalFormattingInParameterizedStringError::IllegalFormattingInParameterizedStringError() {}

	IllegalFormattingInParameterizedStringError::IllegalFormattingInParameterizedStringError(const
			IllegalFormattingInParameterizedStringError& error) : Error(error), TParmError(error) {}

	REDSTRAIN_DEFINE_ERROR(IllegalFormattingInParameterizedStringError) {
		out << "Illegal formatting in parameterized string";
	}

}}
