#ifndef REDSTRAIN_MOD_TEXT_UNEXPECTEDENDOFFORMATSTRINGERROR_HPP
#define REDSTRAIN_MOD_TEXT_UNEXPECTEDENDOFFORMATSTRINGERROR_HPP

#include "FormatStringSyntaxError.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API UnexpectedEndOfFormatStringError : public FormatStringSyntaxError {

	  public:
		UnexpectedEndOfFormatStringError(size_t);
		UnexpectedEndOfFormatStringError(const UnexpectedEndOfFormatStringError&);

		REDSTRAIN_DECLARE_ERROR(UnexpectedEndOfFormatStringError)

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_UNEXPECTEDENDOFFORMATSTRINGERROR_HPP */
