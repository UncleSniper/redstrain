#ifndef REDSTRAIN_MOD_CALENDAR_FORMATSTRINGSYNTAXERROR_HPP
#define REDSTRAIN_MOD_CALENDAR_FORMATSTRINGSYNTAXERROR_HPP

#include <redstrain/util/types.hpp>

#include "FormattingError.hpp"

namespace redengine {
namespace calendar {

	class REDSTRAIN_CALENDAR_API FormatStringSyntaxError : public FormattingError {

	  private:
		const util::MemorySize offset;

	  public:
		FormatStringSyntaxError(util::MemorySize);
		FormatStringSyntaxError(const FormatStringSyntaxError&);

		inline util::MemorySize getOffset() const {
			return offset;
		}

		REDSTRAIN_DECLARE_ABSTRACT_ERROR(FormatStringSyntaxError)

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_FORMATSTRINGSYNTAXERROR_HPP */
