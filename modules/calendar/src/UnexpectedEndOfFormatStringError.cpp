#include "UnexpectedEndOfFormatStringError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace calendar {

	UnexpectedEndOfFormatStringError::UnexpectedEndOfFormatStringError(MemorySize offset)
			: FormatStringSyntaxError(offset) {}

	UnexpectedEndOfFormatStringError::UnexpectedEndOfFormatStringError(const UnexpectedEndOfFormatStringError& error)
			: Error(error), FormatStringSyntaxError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnexpectedEndOfFormatStringError) {
		out << "Unexpected end of format string at offset " << getOffset();
	}

}}
