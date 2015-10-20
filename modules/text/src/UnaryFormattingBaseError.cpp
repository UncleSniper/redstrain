#include "UnaryFormattingBaseError.hpp"

namespace redengine {
namespace text {

	UnaryFormattingBaseError::UnaryFormattingBaseError() {}

	UnaryFormattingBaseError::UnaryFormattingBaseError(const UnaryFormattingBaseError& error)
			: FormattingError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnaryFormattingBaseError) {
		out << "Will not format numbers as unary";
	}

}}
