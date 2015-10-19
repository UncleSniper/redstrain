#ifndef REDSTRAIN_MOD_TEXT_DEFAULTFORMATTINGRENDITION_HPP
#define REDSTRAIN_MOD_TEXT_DEFAULTFORMATTINGRENDITION_HPP

namespace redengine {
namespace text {

	template<typename CharT>
	class DefaultFormattingRendition {

	  public:
		typedef CharT Char;

	  public:
		static const CharT POSITIVE_SIGN = static_cast<CharT>('+');
		static const CharT NEGATIVE_SIGN = static_cast<CharT>('-');
		static const CharT DECIMAL_POINT = static_cast<CharT>('.');
		static const CharT GROUP_SEPARATOR = static_cast<CharT>(',');
		static const CharT FILL_CHARACTER = static_cast<CharT>(' ');

	  public:
		static inline CharT digit(unsigned value, bool upperCase) {
			return static_cast<CharT>((upperCase
					? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz")[value]);
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTFORMATTINGRENDITION_HPP */
