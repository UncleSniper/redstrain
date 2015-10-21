#ifndef REDSTRAIN_MOD_TEXT_UNEXPECTEDFORMATSTRINGCHARACTERERROR_HPP
#define REDSTRAIN_MOD_TEXT_UNEXPECTEDFORMATSTRINGCHARACTERERROR_HPP

#include "FormatStringSyntaxError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UnexpectedFormatStringCharacterError : public FormatStringSyntaxError {

	  public:
		enum Expected {
			EXP_DIGIT,
			EXP_WIDTH_MODIFIER,
			EXP_MODIFIER_OR_DIRECTIVE,
			EXP_TERMINATOR,
			EXP_EXPLICIT_MODIFIER,
			EXP_SIGN_STYLE,
			EXP_CONDITION,
			EXP_CONVERSION,
			EXP_CONDITION_OPERAND_SEPARATOR,
			EXP_SUBGROUP_INITIATOR,
			EXP_SUBGROUP_TERMINATOR,
			EXP_GENERATOR_TYPE
		};

	  private:
		const Expected expected;

	  public:
		UnexpectedFormatStringCharacterError(Expected, size_t);
		UnexpectedFormatStringCharacterError(const UnexpectedFormatStringCharacterError&);

		inline Expected getExpected() const {
			return expected;
		}

		REDSTRAIN_DECLARE_ERROR(UnexpectedFormatStringCharacterError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UNEXPECTEDFORMATSTRINGCHARACTERERROR_HPP */
