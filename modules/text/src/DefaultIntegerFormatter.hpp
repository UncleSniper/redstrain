#ifndef REDSTRAIN_MOD_TEXT_DEFAULTINTEGERFORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_DEFAULTINTEGERFORMATTER_HPP

#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/IntegerBits.hpp>
#include <redstrain/util/IntegerBounds.hpp>

#include "FormattingOptions.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class DefaultIntegerFormatter {

	  public:
		typedef CharT Char;
		typedef std::basic_string<CharT> String;

	  public:
		template<typename IntegerT, typename RenditionT>
		static String formatInteger(IntegerT value,
				const FormattingOptions<CharT, RenditionT>& options = FormattingOptions<CharT, RenditionT>()) {
			const unsigned maxDigits = static_cast<unsigned>(util::integerLog<IntegerT>(
				static_cast<IntegerT>(options.base),
				util::IntegerBounds<IntegerT>::MAX
			)) + 1u;
			String result;
			result.reserve(static_cast<typename String::size_type>(options.integralWidth > maxDigits
					? options.integralWidth : maxDigits));
			bool negative = util::IntegerBits<IntegerT>::isNegative(value);
			unsigned length;
			if(negative) {
				value = -value;
				result += RenditionT::NEGATIVE_SIGN;
				length = 1u;
			}
			else if(options.forcePlus) {
				result += RenditionT::POSITIVE_SIGN;
				length = 1u;
			}
			else
				length = 0u;
			unsigned digits = static_cast<unsigned>(util::integerLog<IntegerT>(
				static_cast<IntegerT>(options.base),
				value
			));
			if(!digits)
				++digits;
			for(; options.integralWidth > length + digits; ++length)
				result += options.integerPadChar;
			CharT buffer[digits];
			CharT* insert = buffer + digits;
			if(value) {
				for(; value; value /= static_cast<IntegerT>(options.base)) {
					*--insert = RenditionT::digit(
						static_cast<unsigned>(value % static_cast<IntegerT>(options.base)),
						options.upperCase
					);
				}
			}
			else
				*--insert = RenditionT::digit(0u, options.upperCase);
			result.append(insert, static_cast<typename String::size_type>(digits));
			return result;
		}


	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTINTEGERFORMATTER_HPP */
