#include "UnexpectedFormatStringCharacterError.hpp"

namespace redengine {
namespace text {

	UnexpectedFormatStringCharacterError::UnexpectedFormatStringCharacterError(Expected expected, size_t offset)
			: FormatStringSyntaxError(offset), expected(expected) {}

	UnexpectedFormatStringCharacterError::UnexpectedFormatStringCharacterError(const
			UnexpectedFormatStringCharacterError& error)
			: FormatStringSyntaxError(error), expected(error.expected) {}

	REDSTRAIN_DEFINE_ERROR(UnexpectedFormatStringCharacterError) {
		out << "Unexpected character in format string at offset " << getOffset() << ", expected ";
		switch(expected) {
			case EXP_DIGIT:
				out << "digit";
				break;
			case EXP_WIDTH_MODIFIER:
				out << "width modifier";
				break;
			case EXP_MODIFIER_OR_DIRECTIVE:
				out << "modifier or directive";
				break;
			case EXP_TERMINATOR:
				out << "terminator symbol";
				break;
			default:
				out << "<Uh oh, unrecognized enum constant...>";
				break;
		}
	}

}}
