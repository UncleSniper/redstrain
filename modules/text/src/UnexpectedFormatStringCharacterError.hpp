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
			EXP_TERMINATOR
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
