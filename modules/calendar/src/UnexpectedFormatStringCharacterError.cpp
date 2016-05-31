#include "UnexpectedFormatStringCharacterError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace calendar {

	UnexpectedFormatStringCharacterError::UnexpectedFormatStringCharacterError(MemorySize offset)
			: FormatStringSyntaxError(offset) {}

	UnexpectedFormatStringCharacterError::UnexpectedFormatStringCharacterError(const
			UnexpectedFormatStringCharacterError& error) : Error(error), FormatStringSyntaxError(error) {}

	REDSTRAIN_DEFINE_ERROR(UnexpectedFormatStringCharacterError) {
		out << "Unexpected character in format string at offset " << getOffset()
				<< ", expected modifier or component";
	}

}}
