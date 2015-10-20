#ifndef REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP

#include <stdint.h>

#include "FormattingStringOption.hpp"
#include "DefaultFormattingRendition.hpp"
#include "FormattingOptionStringEmitter.hpp"

namespace redengine {
namespace text {

	enum FloatFormatStyle {
		FFS_PLAIN,
		FFS_SCIENTIFIC
	};

	enum SignFormatStyle {
		SFS_OMIT,
		SFS_PLUS,
		SFS_FILL
	};

	template<
		typename CharT,
		typename RenditionT = DefaultFormattingRendition<CharT>
	>
	class FormattingOptions {

	  public:
		typedef CharT Char;
		typedef RenditionT Rendition;
		typedef const CharT* (*StringEmitter)(FormattingStringOption);

	  public:
		uint8_t base;
		int32_t integralWidth;
		uint32_t fractionWidth, groupSize;
		bool upperCase, exponentUpperCase, expandFraction;
		CharT integerPadChar, fractionPadChar, fillChar, decimalPoint, groupSeparator;
		FloatFormatStyle floatStyle;
		SignFormatStyle signStyle;
		FormattingOptionStringEmitter<CharT>& stringEmitter;

	  public:
		FormattingOptions(FormattingOptionStringEmitter<CharT>& stringEmitter)
				: base(static_cast<uint8_t>(10u)), integralWidth(static_cast<int32_t>(0)),
				fractionWidth(static_cast<uint32_t>(6u)), groupSize(static_cast<uint32_t>(3u)),
				upperCase(true), exponentUpperCase(false), expandFraction(false),
				integerPadChar(RenditionT::digit(0u, false)), fractionPadChar(integerPadChar),
				fillChar(RenditionT::FILL_CHARACTER), decimalPoint(RenditionT::DECIMAL_POINT),
				groupSeparator(RenditionT::GROUP_SEPARATOR), floatStyle(FFS_PLAIN), signStyle(SFS_OMIT),
				stringEmitter(stringEmitter) {}

		FormattingOptions(FormattingOptionStringEmitter<CharT>& stringEmitter, uint8_t base,
				int32_t integralWidth = static_cast<int32_t>(0),
				uint32_t fractionWidth = static_cast<uint32_t>(6u), SignFormatStyle signStyle = SFS_OMIT,
				CharT integerPadChar = RenditionT::digit(0u, false), bool expandFraction = false,
				CharT fractionPadChar = RenditionT::digit(0u, false), CharT fillChar = RenditionT::FILL_CHARACTER,
				bool upperCase = true, CharT decimalPoint = RenditionT::DECIMAL_POINT,
				FloatFormatStyle floatStyle = FFS_PLAIN, bool exponentUpperCase = false,
				CharT groupSeparator = RenditionT::GROUP_SEPARATOR, uint32_t groupSize = 3u)
				: base(base), integralWidth(integralWidth), fractionWidth(fractionWidth), groupSize(groupSize),
				upperCase(upperCase), exponentUpperCase(exponentUpperCase), expandFraction(expandFraction),
				integerPadChar(integerPadChar), decimalPoint(decimalPoint), groupSeparator(groupSeparator),
				floatStyle(floatStyle), signStyle(SFS_OMIT), stringEmitter(stringEmitter) {}

		FormattingOptions(const FormattingOptions& options) : base(options.base),
				integralWidth(options.integralWidth), fractionWidth(options.fractionWidth),
				groupSize(options.groupSize), upperCase(options.upperCase),
				exponentUpperCase(options.exponentUpperCase), expandFraction(options.expandFraction),
				integerPadChar(options.integerPadChar), fractionPadChar(options.fractionPadChar),
				fillChar(options.fillChar), decimalPoint(options.decimalPoint),
				groupSeparator(options.groupSeparator), floatStyle(options.floatStyle), signStyle(options.signStyle),
				stringEmitter(options.stringEmitter) {}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP */
