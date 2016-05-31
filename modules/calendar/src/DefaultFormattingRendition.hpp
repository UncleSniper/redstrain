#ifndef REDSTRAIN_MOD_CALENDAR_DEFAULTFORMATTINGRENDITION_HPP
#define REDSTRAIN_MOD_CALENDAR_DEFAULTFORMATTINGRENDITION_HPP

namespace redengine {
namespace calendar {

	template<typename CharT>
	class DefaultFormattingRendition {

	  public:
		typedef CharT Char;

	  public:
		static const CharT FILL_CHARACTER = static_cast<CharT>(' ');

	  public:
		static inline CharT digit(unsigned value) {
			return static_cast<CharT>("0123456789"[value]);
		}

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_DEFAULTFORMATTINGRENDITION_HPP */
