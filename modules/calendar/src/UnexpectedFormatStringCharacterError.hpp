#ifndef REDSTRAIN_MOD_CALENDAR_UNEXPECTEDFORMATSTRINGCHARACTERERROR_HPP
#define REDSTRAIN_MOD_CALENDAR_UNEXPECTEDFORMATSTRINGCHARACTERERROR_HPP

#include "FormatStringSyntaxError.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API UnexpectedFormatStringCharacterError : public FormatStringSyntaxError {

	  public:
		UnexpectedFormatStringCharacterError(util::MemorySize);
		UnexpectedFormatStringCharacterError(const UnexpectedFormatStringCharacterError&);

		REDSTRAIN_DECLARE_ERROR(UnexpectedFormatStringCharacterError)

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_UNEXPECTEDFORMATSTRINGCHARACTERERROR_HPP */
