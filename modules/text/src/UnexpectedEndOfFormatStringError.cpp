#include "UnexpectedEndOfFormatStringError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace text {

	UnexpectedEndOfFormatStringError::UnexpectedEndOfFormatStringError(MemorySize offset)
			: FormatStringSyntaxError(offset) {}

	UnexpectedEndOfFormatStringError::UnexpectedEndOfFormatStringError(const UnexpectedEndOfFormatStringError& error)
			: FormatStringSyntaxError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnexpectedEndOfFormatStringError) {
		out << "Unexpected end of format string at offset " << getOffset();
	}

}}
