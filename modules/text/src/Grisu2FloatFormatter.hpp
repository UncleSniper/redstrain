#ifndef REDSTRAIN_MOD_TEXT_GRISU2FLOATFORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_GRISU2FLOATFORMATTER_HPP

#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/FloatTraits.hpp>
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
			String result;
			//TODO
			return result;
		}

	  public:
		template<typename FloatT, typename RenditionT>
		static String formatFloat(FloatT value,
				const FormattingOptions<CharT, RenditionT>& options = FormattingOptions<CharT, RenditionT>()) {
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
			const unsigned exponentWidth = options.floatStyle == FFS_PLAIN
				? 0u : 2u + static_cast<unsigned>(util::IntegerLog<
					int32_t,
					static_cast<int32_t>(10u),
					util::IntegerBounds<int32_t>::MAX
				>::EXPONENT);
			const unsigned maxDigits = static_cast<unsigned>(util::IntegerLog<
				uint64_t,
				static_cast<uint64_t>(10u),
				util::IntegerBounds<uint64_t>::MAX
			>::EXPONENT) * 2u + 2u + exponentWidth;
			const unsigned requestedWidth = static_cast<unsigned>(options.integralWidth < static_cast<int32_t>(0u)
					? -options.integralWidth : options.integralWidth) + static_cast<unsigned>(options.fractionWidth)
					+ 2u + exponentWidth;
			String result;
			result.reserve(static_cast<typename String::size_type>(requestedWidth > maxDigits
					? requestedWidth : maxDigits));
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
			//TODO
			return result;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_GRISU2FLOATFORMATTER_HPP */
