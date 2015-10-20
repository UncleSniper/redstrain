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

	enum FormattingOptionFlags {
		FOF_UPPERCASE_DIGITS   = 001,
		FOF_UPPERCASE_EXPONENT = 002,
		FOF_EXPAND_FRACTION    = 004
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
		static const int DEFAULT_FLAGS = FOF_UPPERCASE_DIGITS;

	  public:
		uint8_t base;
		int32_t integralWidth;
		uint32_t fractionWidth, groupSize;
		int flags;
		CharT integerPadChar, fractionPadChar, fillChar, decimalPoint, groupSeparator;
		FloatFormatStyle floatStyle;
		SignFormatStyle signStyle;
		FormattingOptionStringEmitter<CharT>& stringEmitter;

	  public:
		FormattingOptions(FormattingOptionStringEmitter<CharT>& stringEmitter)
				: base(static_cast<uint8_t>(10u)), integralWidth(static_cast<int32_t>(0)),
				fractionWidth(static_cast<uint32_t>(6u)), groupSize(static_cast<uint32_t>(3u)),
				flags(DEFAULT_FLAGS), integerPadChar(RenditionT::digit(0u, false)), fractionPadChar(integerPadChar),
				fillChar(RenditionT::FILL_CHARACTER), decimalPoint(RenditionT::DECIMAL_POINT),
				groupSeparator(RenditionT::GROUP_SEPARATOR), floatStyle(FFS_PLAIN), signStyle(SFS_OMIT),
				stringEmitter(stringEmitter) {}

		FormattingOptions(FormattingOptionStringEmitter<CharT>& stringEmitter, uint8_t base,
				int32_t integralWidth = static_cast<int32_t>(0),
				uint32_t fractionWidth = static_cast<uint32_t>(6u), SignFormatStyle signStyle = SFS_OMIT,
				int flags = DEFAULT_FLAGS, CharT integerPadChar = RenditionT::digit(0u, false),
				CharT fractionPadChar = RenditionT::digit(0u, false), CharT fillChar = RenditionT::FILL_CHARACTER,
				CharT decimalPoint = RenditionT::DECIMAL_POINT, FloatFormatStyle floatStyle = FFS_PLAIN,
				CharT groupSeparator = RenditionT::GROUP_SEPARATOR, uint32_t groupSize = 3u)
				: base(base), integralWidth(integralWidth), fractionWidth(fractionWidth), groupSize(groupSize),
				flags(flags), integerPadChar(integerPadChar), decimalPoint(decimalPoint),
				groupSeparator(groupSeparator), floatStyle(floatStyle), signStyle(SFS_OMIT),
				stringEmitter(stringEmitter) {}

		FormattingOptions(const FormattingOptions& options) : base(options.base),
				integralWidth(options.integralWidth), fractionWidth(options.fractionWidth),
				groupSize(options.groupSize), flags(options.flags), integerPadChar(options.integerPadChar),
				fractionPadChar(options.fractionPadChar), fillChar(options.fillChar),
				decimalPoint(options.decimalPoint), groupSeparator(options.groupSeparator),
				floatStyle(options.floatStyle), signStyle(options.signStyle), stringEmitter(options.stringEmitter) {}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP */
