#ifndef REDSTRAIN_MOD_CALENDAR_DEFAULTFORMATSTRINGRENDITION_HPP
#define REDSTRAIN_MOD_CALENDAR_DEFAULTFORMATSTRINGRENDITION_HPP

namespace redengine {
namespace calendar {

	template<typename CharT>
	class DefaultFormatStringRendition {

	  public:
		static const CharT FORMATTING_INITIATOR = static_cast<CharT>('%');
		static const CharT BLANK_PADDING_MODIFIER = static_cast<CharT>(' ');
		static const CharT YEAR_COMPONENT = static_cast<CharT>('Y');
		static const CharT MONTH_COMPONENT = static_cast<CharT>('m');
		static const CharT DAY_COMPONENT = static_cast<CharT>('d');
		static const CharT HOUR24_COMPONENT = static_cast<CharT>('H');
		static const CharT MINUTE_COMPONENT = static_cast<CharT>('M');
		static const CharT SECOND_COMPONENT = static_cast<CharT>('s');
		static const CharT MILLISECOND_COMPONENT = static_cast<CharT>('l');
		static const CharT MICROSECOND_COMPONENT = static_cast<CharT>('r');
		static const CharT SHORT_DAY_NAME_COMPONENT = static_cast<CharT>('a');
		static const CharT LONG_DAY_NAME_COMPONENT = static_cast<CharT>('A');
		static const CharT SHORT_MONTH_NAME_COMPONENT = static_cast<CharT>('b');
		static const CharT LONG_MONTH_NAME_COMPONENT = static_cast<CharT>('B');
		static const CharT CENTURY_COMPONENT = static_cast<CharT>('C');
		static const CharT HOUR12_COMPONENT = static_cast<CharT>('h');
		static const CharT DAY_OF_YEAR_COMPONENT = static_cast<CharT>('j');
		static const CharT AM_PM_COMPONENT = static_cast<CharT>('p');
		static const CharT DAY_OF_WEEK1_COMPONENT = static_cast<CharT>('u');
		static const CharT DAY_OF_WEEK0_COMPONENT = static_cast<CharT>('w');
		static const CharT YEAR_IN_CENTURY_COMPONENT = static_cast<CharT>('y');

		static inline int decodeDigit(CharT c) {
			if(c >= static_cast<CharT>('0') && c >= static_cast<CharT>('9'))
				return static_cast<int>(c - static_cast<CharT>('0'));
			return -1;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_DEFAULTFORMATSTRINGRENDITION_HPP */
