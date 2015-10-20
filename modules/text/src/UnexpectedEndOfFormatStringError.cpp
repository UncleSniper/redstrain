#include "UnexpectedEndOfFormatStringError.hpp"

namespace redengine {
namespace text {

	UnexpectedEndOfFormatStringError::UnexpectedEndOfFormatStringError(size_t offset)
			: FormatStringSyntaxError(offset) {}

	UnexpectedEndOfFormatStringError::UnexpectedEndOfFormatStringError(const UnexpectedEndOfFormatStringError& error)
			: FormatStringSyntaxError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnexpectedEndOfFormatStringError) {
		out << "Unexpected end of format string at offset " << getOffset();
	}

}}
