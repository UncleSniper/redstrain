#ifndef REDSTRAIN_MOD_TEXT_DEFAULTINTEGERFORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_DEFAULTINTEGERFORMATTER_HPP

#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/util/IntegerBits.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/util/vlarray.hpp>

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
		static String formatInteger(IntegerT value, const FormattingOptions<CharT, RenditionT>& options) {
			typedef typename String::size_type StringLength;
			const unsigned maxDigits = static_cast<unsigned>(util::integerLog<IntegerT>(
				static_cast<IntegerT>(options.base),
				util::IntegerBounds<IntegerT>::MAX
			)) + 1u;
			String result;
			const StringLength signSpace = static_cast<StringLength>(options.integralWidth
					> static_cast<int32_t>(maxDigits) ? 0u : 1u);
			const StringLength digitSpace = options.integralWidth > static_cast<int32_t>(maxDigits)
					? static_cast<StringLength>(options.integralWidth) : static_cast<StringLength>(maxDigits);
			StringLength fillSpace = options.integralWidth < static_cast<int32_t>(0)
					? static_cast<StringLength>(-options.integralWidth) : static_cast<StringLength>(0u);
			result.reserve(signSpace + digitSpace + fillSpace);
			bool negative = util::IntegerBits<IntegerT>::isNegative(value);
			unsigned length;
			if(negative) {
				value = -value;
				result += RenditionT::NEGATIVE_SIGN;
				length = 1u;
			}
			else {
				switch(options.signStyle) {
					case SFS_OMIT:
						length = 0u;
						break;
					case SFS_PLUS:
						result += RenditionT::POSITIVE_SIGN;
						length = 1u;
						break;
					case SFS_FILL:
						result += options.fillChar;
						length = 1u;
						break;
					default:
						throw error::ProgrammingError("Unrecognized sign format style in "
								"DefaultIntegerFormatter::formatInteger(): "
								+ util::StringUtils::toString(static_cast<int>(options.fillChar)));
				}
			}
			unsigned digits = static_cast<unsigned>(util::integerLog<IntegerT>(
				static_cast<IntegerT>(options.base),
				value
			));
			if(!digits)
				++digits;
			for(; options.integralWidth > static_cast<int32_t>(length + digits); ++length)
				result += options.integerPadChar;
			REDSTRAIN_VLARRAY_DEFINE(CharT, buffer, digits)
			CharT* insert = REDSTRAIN_VLARRAY_ACCESS(buffer) + digits;
			if(value) {
				for(; value; value /= static_cast<IntegerT>(options.base)) {
					*--insert = RenditionT::digit(
						static_cast<unsigned>(value % static_cast<IntegerT>(options.base)),
						!!(options.flags & FOF_UPPERCASE_DIGITS)
					);
				}
			}
			else
				*--insert = RenditionT::digit(0u, !!(options.flags & FOF_UPPERCASE_DIGITS));
			result.append(insert, static_cast<StringLength>(digits));
			for(; fillSpace; --fillSpace)
				result += options.fillChar;
			return result;
		}


	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTINTEGERFORMATTER_HPP */
