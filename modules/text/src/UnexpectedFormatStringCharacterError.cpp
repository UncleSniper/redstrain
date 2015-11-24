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
			case EXP_EXPLICIT_MODIFIER:
				out << "explicit modifier";
				break;
			case EXP_SIGN_STYLE:
				out << "sign style";
				break;
			case EXP_CONDITION:
				out << "condition";
				break;
			case EXP_CONVERSION:
				out << "conversion";
				break;
			case EXP_CONDITION_OPERAND_SEPARATOR:
				out << "condition operand separator";
				break;
			case EXP_SUBGROUP_INITIATOR:
				out << "subgroup initiator";
				break;
			case EXP_SUBGROUP_TERMINATOR:
				out << "subgroup terminator";
				break;
			case EXP_GENERATOR_TYPE:
				out << "generator type";
				break;
			case EXP_CONDITION_GROUP_TERMINATOR:
				out << "condition group terminator";
				break;
			default:
				out << "<Uh oh, unrecognized enum constant...>";
				break;
		}
	}

}}
