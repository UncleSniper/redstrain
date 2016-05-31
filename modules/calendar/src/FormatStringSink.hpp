#ifndef REDSTRAIN_MOD_CALENDAR_FORMATSTRINGSINK_HPP
#define REDSTRAIN_MOD_CALENDAR_FORMATSTRINGSINK_HPP

#include <redstrain/util/types.hpp>

#include "FormattingOptions.hpp"

namespace redengine {
namespace calendar {

	template<typename CharT>
	class FormatStringSink {

	  public:
		enum Component {
			COMP_YEAR,
			COMP_MONTH,
			COMP_DAY,
			COMP_HOUR24,
			COMP_MINUTE,
			COMP_SECOND,
			COMP_MILLISECOND,
			COMP_MICROSECOND,
			COMP_SHORT_DAY_NAME,
			COMP_LONG_DAY_NAME,
			COMP_SHORT_MONTH_NAME,
			COMP_LONG_MONTH_NAME,
			COMP_CENTURY,
			COMP_HOUR12,
			COMP_DAY_OF_YEAR,
			COMP_AM_PM,
			COMP_DAY_OF_WEEK1,
			COMP_DAY_OF_WEEK0,
			COMP_YEAR_IN_CENTURY
		};

	  public:
		FormatStringSink() {}
		FormatStringSink(const FormatStringSink&) {}
		virtual ~FormatStringSink() {}

		virtual void verbatimText(const CharT*, util::MemorySize) = 0;
		virtual void formatting(const FormattingOptions&, Component) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_FORMATSTRINGSINK_HPP */
