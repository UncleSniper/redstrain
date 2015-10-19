#ifndef REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP

#include <stdint.h>

#include "DefaultFormattingRendition.hpp"

namespace redengine {
namespace text {

	enum FloatFormatStyle {
		FFS_PLAIN,
		FFS_SCIENTIFIC
	};

	template<
		typename CharT,
		typename RenditionT = DefaultFormattingRendition<CharT>
	>
	class FormattingOptions {

	  public:
		typedef CharT Char;
		typedef RenditionT Rendition;

	  public:
		uint8_t base;
		int32_t integralWidth;
		uint32_t fractionWidth, groupSize;
		bool forcePlus, upperCase, exponentUpperCase;
		CharT integerPadChar, fractionPadChar, fillChar, decimalPoint, groupSeparator;
		FloatFormatStyle floatStyle;

	  public:
		FormattingOptions() : base(static_cast<uint8_t>(10u)), integralWidth(static_cast<int32_t>(0u)),
				fractionWidth(static_cast<uint32_t>(1u)), groupSize(static_cast<uint32_t>(3u)),
				forcePlus(false), upperCase(true), exponentUpperCase(false),
				integerPadChar(RenditionT::digit(0u, false)), fractionPadChar(integerPadChar),
				fillChar(RenditionT::FILL_CHARACTER), decimalPoint(RenditionT::DECIMAL_POINT),
				groupSeparator(RenditionT::GROUP_SEPARATOR), floatStyle(FFS_PLAIN) {}

		FormattingOptions(uint8_t base, int32_t integralWidth = 0u, uint32_t fractionWidth = 1u,
				bool forcePlus = false, CharT integerPadChar = RenditionT::digit(0u, false),
				CharT fractionPadChar = RenditionT::digit(0u, false), CharT fillChar = RenditionT::FILL_CHARACTER,
				bool upperCase = true, CharT decimalPoint = RenditionT::DECIMAL_POINT,
				FloatFormatStyle floatStyle = FFS_PLAIN, bool exponentUpperCase = false,
				CharT groupSeparator = RenditionT::GROUP_SEPARATOR, uint32_t groupSize = 3u)
				: base(base), integralWidth(integralWidth), fractionWidth(fractionWidth), groupSize(groupSize),
				forcePlus(forcePlus), upperCase(upperCase), exponentUpperCase(exponentUpperCase),
				integerPadChar(integerPadChar), decimalPoint(decimalPoint), groupSeparator(groupSeparator),
				floatStyle(floatStyle) {}

		FormattingOptions(const FormattingOptions& options) : base(options.base),
				integralWidth(options.integralWidth), fractionWidth(options.fractionWidth),
				groupSize(options.groupSize), forcePlus(options.forcePlus), upperCase(options.upperCase),
				exponentUpperCase(options.exponentUpperCase), integerPadChar(options.integerPadChar),
				fractionPadChar(options.fractionPadChar), fillChar(options.fillChar),
				decimalPoint(options.decimalPoint), groupSeparator(options.groupSeparator),
				floatStyle(options.floatStyle) {}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP */
