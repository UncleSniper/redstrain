#ifndef REDSTRAIN_MOD_TEXT_GRISU2FLOATFORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_GRISU2FLOATFORMATTER_HPP

#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/FloatTraits.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>
#include <redstrain/util/mathwrap.hpp>

#include "Grisu.hpp"
#include "FormattingOptions.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class Grisu2FloatFormatter {

	  public:
		typedef CharT Char;
		typedef std::basic_string<CharT> String;

	  private:
		template<typename FloatT, typename RenditionT>
		static String formatSpecialFloat(FloatT value, util::FloatingPointClass fpclass,
				const FormattingOptions<CharT, RenditionT>& options) {
			typedef typename String::size_type StringLength;
			String result;
			if(fpclass == util::FPC_ZERO) {
				const StringLength signSpace = static_cast<StringLength>(options.integralWidth
						> static_cast<int32_t>(1) ? 0u : 1u);
				const StringLength intSpace = options.integralWidth > static_cast<int32_t>(1)
						? static_cast<StringLength>(options.integralWidth) : static_cast<StringLength>(1u);
				const StringLength dotSpace = static_cast<StringLength>(options.fractionWidth ? 1u : 0u);
				const StringLength fracSpace = options.fractionWidth ? (options.expandFraction
						? static_cast<StringLength>(options.fractionWidth) : static_cast<StringLength>(1u))
						: static_cast<StringLength>(0u);
				const StringLength expSpace = static_cast<StringLength>(options.floatStyle == FFS_PLAIN ? 0u : 4u);
				StringLength fillSpace = options.integralWidth < -static_cast<int32_t>(signSpace + intSpace)
						? static_cast<StringLength>(-options.integralWidth
						- static_cast<int32_t>(signSpace + intSpace)) : static_cast<StringLength>(0u);
				result.reserve(signSpace + intSpace + dotSpace + fracSpace + expSpace + fillSpace);
				unsigned length;
				if(value < static_cast<FloatT>(0)) {
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
									"Grisu2FloatFormatter::formatSpecialFloat(): "
									+ util::StringUtils::toString(static_cast<int>(options.fillChar)));
					}
				}
				for(; options.integralWidth > static_cast<int32_t>(length + 1u); ++length)
					result += options.integerPadChar;
				result += RenditionT::digit(0u, options.upperCase);
				if(options.fractionWidth) {
					result += options.decimalPoint;
					result += RenditionT::digit(0u, options.upperCase);
					if(options.expandFraction) {
						uint32_t u;
						for(u = static_cast<uint32_t>(1u); u < options.fractionWidth; ++u)
							result += options.fractionPadChar;
					}
				}
				if(options.floatStyle == FFS_SCIENTIFIC) {
					if(options.exponentUpperCase)
						result += RenditionT::UPPERCASE_EXPONENT;
					else
						result += RenditionT::LOWERCASE_EXPONENT;
					result += RenditionT::POSITIVE_SIGN;
					CharT zero = RenditionT::digit(0u, options.upperCase);
					result += zero;
					result += zero;
				}
				for(; fillSpace; --fillSpace)
					result += options.fillChar;
				return result;
			}
			const unsigned frontWidth = options.integralWidth < static_cast<int32_t>(0)
					? 0u : static_cast<unsigned>(options.integralWidth);
			unsigned backWidth = (options.fractionWidth
					? static_cast<unsigned>(options.fractionWidth) + 1u : 0u)
					+ (options.integralWidth < static_cast<int32_t>(0)
					? static_cast<unsigned>(-options.integralWidth) : 0u)
					+ (options.floatStyle == FFS_PLAIN ? 0u : 4u);
			FormattingOptionStringEmitter<CharT>& stringEmitter
					= const_cast<FormattingOptionStringEmitter<CharT>&>(options.stringEmitter);
			String name;
			switch(fpclass) {
				case util::FPC_NAN:
					name = stringEmitter.emitString(FSO_NAN);
					break;
				case util::FPC_INFINITE:
					name = stringEmitter.emitString(value < static_cast<FloatT>(0)
							? FSO_NEGATIVE_INFINITY : FSO_POSITIVE_INFINITY);
					break;
				case util::FPC_ZERO:
				case util::FPC_SUBNORMAL:
				case util::FPC_NORMAL:
				default:
					throw error::ProgrammingError("Unexpected floating-point class in "
							"Grisu2FloatFormatter::formatSpecialFloat(): "
							+ util::StringUtils::toString(static_cast<int>(fpclass)));
			}
			typename String::size_type nlen = name.length();
			result.reserve((nlen > static_cast<typename String::size_type>(frontWidth)
					? nlen : static_cast<typename String::size_type>(frontWidth))
					+ static_cast<typename String::size_type>(backWidth));
			for(; nlen < static_cast<typename String::size_type>(frontWidth); ++nlen)
				result += options.fillChar;
			result += name;
			for(; backWidth; --backWidth)
				result += options.fillChar;
			return result;
		}

	  public:
		template<typename FloatT, typename RenditionT>
		static String formatFloat(FloatT value, const FormattingOptions<CharT, RenditionT>& options) {
			typedef typename String::size_type StringLength;
			typedef util::FloatTraits<FloatT> Traits;
			typedef typename Traits::Mantissa Mantissa;
			typedef typename Traits::Exponent Exponent;
			util::FloatingPointClass fpclass = util::FloatingPointClassify<FloatT>::classify(value);
			switch(fpclass) {
				case util::FPC_NAN:
				case util::FPC_INFINITE:
				case util::FPC_ZERO:
					return formatSpecialFloat<FloatT, RenditionT>(value, fpclass, options);
				case util::FPC_SUBNORMAL:
				case util::FPC_NORMAL:
				default:
					break;
			}
			bool negative;
			if(value < static_cast<FloatT>(0)) {
				negative = true;
				value = -value;
			}
			else
				negative = false;
			Mantissa mantissa;
			Exponent exponent;
			Traits::mantissaAndExponent(value, mantissa, exponent);
			Grisu::DigitSequence dseq;
			int32_t g2exp = Grisu::grisu2(Grisu::FakeFloat(
				static_cast<uint64_t>(mantissa),
				static_cast<int32_t>(exponent) - static_cast<int32_t>(Traits::MANTISSA_BITS + 1u)
			), dseq);
			unsigned digits = static_cast<unsigned>(dseq.length());
			// FFS_SCIENTIFIC means: shift ye olde decimal point to just right of the front digit
			int32_t expExp;  // exponent to print for FFS_SCIENTIFIC
			if(options.floatStyle == FFS_SCIENTIFIC) {
				// Shifting decimal point deltaExp places to the left:
				//         digits - 1
				//    |<---------------|
				//   d.ddddddd.ddddddddd
				//            |<-------|
				//               -g2exp
				int32_t deltaExp = static_cast<int32_t>(digits - 1u) + g2exp;
				g2exp -= deltaExp;
				expExp = -deltaExp;
			}
			// Possible situations:
			//   (a)  ddd.dd => g2exp < 0 && -g2exp < digits
			//   (b)  ddddd. => g2exp >= 0
			//   (c)  .ddddd => -g2exp >= digits
			unsigned intDrawn, intPadded, fracDrawn, fracPadded;
			if(g2exp >= static_cast<int32_t>(0)) {
				// situation (b)
				intDrawn = digits;
				fracDrawn = 0u;
				intPadded = static_cast<unsigned>(g2exp);
				fracPadded = 1u;
			}
			else if(-g2exp >= static_cast<int32_t>(digits)) {
				// situation (c)
				intDrawn = 0u;
				fracDrawn = digits;
				intPadded = 1u;
				fracPadded = static_cast<unsigned>(-g2exp) - digits;
			}
			else {
				// situation (a)
				fracDrawn = static_cast<unsigned>(-g2exp);
				intDrawn = digits - fracDrawn;
				fracPadded = intPadded = 0u;
			}
			unsigned intDigits = intDrawn + intPadded, fracDigits = fracDrawn + fracPadded;
			String result;
			const StringLength signSpace = static_cast<StringLength>(options.integralWidth
					> static_cast<int32_t>(intDigits) ? 0u : 1u);
			const StringLength intSpace = options.integralWidth > static_cast<int32_t>(intDigits)
					? static_cast<StringLength>(options.integralWidth) : static_cast<StringLength>(intDigits);
			const StringLength dotSpace = static_cast<StringLength>(options.fractionWidth ? 1u : 0u);
			const StringLength fracSpace = options.fractionWidth ? (options.expandFraction
					? static_cast<StringLength>(options.fractionWidth)
					: (fracDigits < static_cast<unsigned>(options.fractionWidth)
					? static_cast<StringLength>(fracDigits) : static_cast<StringLength>(options.fractionWidth)))
					: static_cast<StringLength>(0u);
			StringLength expSpace;
			unsigned expDigits;
			if(options.floatStyle == FFS_PLAIN)
				expSpace = static_cast<StringLength>(0u);
			else {
				expDigits = static_cast<unsigned>(util::integerLog<int32_t>(static_cast<int32_t>(10),
						expExp));
				expSpace = static_cast<StringLength>(2u + (expDigits > 2u ? expDigits : 2u));
			}
			StringLength fillSpace = options.integralWidth < -static_cast<int32_t>(signSpace + intSpace)
					? static_cast<StringLength>(-options.integralWidth
					- static_cast<int32_t>(signSpace + intSpace)) : static_cast<StringLength>(0u);
			result.reserve(signSpace + intSpace + dotSpace + fracSpace + expSpace + fillSpace);
			unsigned length;
			if(negative)
				result += RenditionT::NEGATIVE_SIGN;
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
								"Grisu2FloatFormatter::formatFloat(): "
								+ util::StringUtils::toString(static_cast<int>(options.fillChar)));
				}
			}
			for(; options.integralWidth > static_cast<int32_t>(length + intDigits); ++length)
				result += options.integerPadChar;
			Grisu::DigitSequence::const_iterator dbegin(dseq.begin());
			for(; intDrawn; --intDrawn, ++dbegin)
				result += RenditionT::digit(static_cast<unsigned>(*dbegin), options.upperCase);
			CharT zero(RenditionT::digit(0u, options.upperCase));
			for(; intPadded; --intPadded)
				result += zero;
			if(options.fractionWidth) {
				result += options.decimalPoint;
				length = 0u;
				for(; fracPadded; --fracPadded) {
					if(++length == static_cast<unsigned>(options.fractionWidth)) {
						// this is the last printed decimal place => round
						if(fracPadded > 1u)  // next digit would be zero due to padding
							result += zero;
						else if(static_cast<unsigned>(*dbegin) >= 5u)  // round up
							result += RenditionT::digit(1u, options.upperCase);
						else  // round down
							result += zero;
						break;
					}
					result += zero;
				}
				if(length < static_cast<unsigned>(options.fractionWidth)) {
					for(; fracDrawn; --fracDrawn, ++dbegin) {
						if(++length == static_cast<unsigned>(options.fractionWidth)) {
							// this is the last printed decimal place => round
							unsigned thisPlace = static_cast<unsigned>(*dbegin);
							if(fracDrawn == 1u)  // no more digits; next would be zero
								result += RenditionT::digit(thisPlace, options.upperCase);
							else if(static_cast<unsigned>(*++dbegin) < 5u)  // round down
								result += RenditionT::digit(thisPlace, options.upperCase);
							else {  // round up
								if(thisPlace < 9u)
									++thisPlace;
								else {
									// Now we are, of course, royally screwed. We would
									// have move back through the already printed parts
									// as the carry propagates, cutting the whole thing
									// off somewhere and re-filling with zeroes... This
									// might actually affect the integral part, yadda
									// yadda. There's just no way in <insert preferred
									// place of eternal damnation> we're doing that;
									// the user will just have to live with an implicit
									// round-down here. Who's gonna know, anyway...? :P
								}
								result += RenditionT::digit(thisPlace, options.upperCase);
							}
							break;
						}
						result += RenditionT::digit(static_cast<unsigned>(*dbegin), options.upperCase);
					}
				}
				if(options.expandFraction) {
					for(; length < static_cast<unsigned>(options.expandFraction); ++length)
						result += options.fractionPadChar;
				}
			}
			if(options.floatStyle == FFS_SCIENTIFIC) {
				if(options.exponentUpperCase)
					result += RenditionT::UPPERCASE_EXPONENT;
				else
					result += RenditionT::LOWERCASE_EXPONENT;
				if(expExp < static_cast<int32_t>(0)) {
					expExp = -expExp;
					result += RenditionT::NEGATIVE_SIGN;
				}
				else
					result += RenditionT::POSITIVE_SIGN;
				if(!expDigits)
					++expDigits;
				CharT buffer[expDigits];
				CharT* insert = buffer + expDigits;
				if(expExp) {
					for(; expExp; expExp /= static_cast<int32_t>(10)) {
						*--insert = RenditionT::digit(
							static_cast<unsigned>(expExp % static_cast<int32_t>(10)),
							options.upperCase
						);
					}
				}
				else
					*--insert = zero;
				result.append(insert, static_cast<StringLength>(expDigits));
				for(; expDigits < 2u; ++expDigits)
					result += zero;
			}
			for(; fillSpace; --fillSpace)
				result += options.fillChar;
			return result;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_GRISU2FLOATFORMATTER_HPP */
