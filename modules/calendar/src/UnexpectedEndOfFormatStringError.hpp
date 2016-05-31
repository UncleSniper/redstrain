#ifndef REDSTRAIN_MOD_CALENDAR_UNEXPECTEDENDOFFORMATSTRINGERROR_HPP
#define REDSTRAIN_MOD_CALENDAR_UNEXPECTEDENDOFFORMATSTRINGERROR_HPP

#include "FormatStringSyntaxError.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API UnexpectedEndOfFormatStringError : public FormatStringSyntaxError {

	  public:
		UnexpectedEndOfFormatStringError(util::MemorySize);
		UnexpectedEndOfFormatStringError(const UnexpectedEndOfFormatStringError&);

		REDSTRAIN_DECLARE_ERROR(UnexpectedEndOfFormatStringError)

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_UNEXPECTEDENDOFFORMATSTRINGERROR_HPP */
