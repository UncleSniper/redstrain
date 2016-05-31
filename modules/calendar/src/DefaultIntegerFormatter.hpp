#ifndef REDSTRAIN_MOD_CALENDAR_DEFAULTINTEGERFORMATTER_HPP
#define REDSTRAIN_MOD_CALENDAR_DEFAULTINTEGERFORMATTER_HPP

#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/IntegerBounds.hpp>

#include "FormattingOptions.hpp"

namespace redengine {
namespace calendar {

	template<typename CharT>
	class DefaultIntegerFormatter {

	  public:
		typedef CharT Char;
		typedef std::basic_string<CharT> String;

	  public:
		template<typename IntegerT, typename RenditionT>
		static String formatInteger(IntegerT value, const FormattingOptions& options) {
			typedef typename String::size_type StringLength;
			const unsigned maxDigits = static_cast<unsigned>(util::IntegerLog<
				IntegerT,
				static_cast<IntegerT>(10),
				util::IntegerBounds<IntegerT>::MAX
			>::EXPONENT);
			String result;
			const StringLength digitSpace = static_cast<StringLength>(options.width > maxDigits
					? options.width : maxDigits);
			result.reserve(digitSpace);
			unsigned digits = static_cast<unsigned>(util::integerLog<IntegerT>(static_cast<IntegerT>(10), value));
			if(!digits)
				++digits;
			CharT padChar = options.padding == FormattingOptions::PAD_ZERO
					? RenditionT::digit(0u) : RenditionT::FILL_CHARACTER;
			unsigned length = 0u;
			for(; options.width > length + digits; ++length)
				result += padChar;
			CharT buffer[maxDigits];
			CharT* insert = buffer + digits;
			if(value) {
				for(; value; value /= static_cast<IntegerT>(10))
					*--insert = RenditionT::digit(static_cast<unsigned>(value % static_cast<IntegerT>(10)));
			}
			else
				*--insert = RenditionT::digit(0u);
			result.append(insert, static_cast<StringLength>(digits));
			return result;
		}

	};

}}

#endif /* REDSTRAIN_MOD_CALENDAR_DEFAULTINTEGERFORMATTER_HPP */
