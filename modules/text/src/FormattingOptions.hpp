#ifndef REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP

#include "DefaultFormattingRendition.hpp"

namespace redengine {
namespace text {

	template<
		typename CharT,
		typename RenditionT = DefaultFormattingRendition<CharT>
	>
	class FormattingOptions {

	  public:
		typedef CharT Char;
		typedef RenditionT Rendition;

	  public:
		unsigned base, mantissaWidth, fractionWidth, groupSize;
		bool forcePlus, upperCase;
		CharT padChar, decimalPoint, groupSeparator;

	  public:
		FormattingOptions() : base(10u), mantissaWidth(0u), fractionWidth(1u), groupSize(3u),
				forcePlus(false), upperCase(true), padChar(RenditionT::digit(0u, false)),
				decimalPoint(RenditionT::DECIMAL_POINT), groupSeparator(RenditionT::GROUP_SEPARATOR) {}

		FormattingOptions(unsigned base, unsigned mantissaWidth = 0u, unsigned fractionWidth = 1u,
				bool forcePlus = false, CharT padChar = RenditionT::digit(0u, false), bool upperCase = true,
				CharT decimalPoint = RenditionT::DECIMAL_POINT, CharT groupSeparator = RenditionT::GROUP_SEPARATOR,
				unsigned groupSize = 3u)
				: base(base), mantissaWidth(mantissaWidth), fractionWidth(fractionWidth), groupSize(groupSize),
				forcePlus(forcePlus), upperCase(upperCase), padChar(padChar), decimalPoint(decimalPoint),
				groupSeparator(groupSeparator) {}

		FormattingOptions(const FormattingOptions& options) : base(options.base),
				mantissaWidth(options.mantissaWidth), fractionWidth(options.fractionWidth),
				groupSize(options.groupSize), forcePlus(options.forcePlus), upperCase(options.upperCase),
				padChar(options.padChar), decimalPoint(options.decimalPoint),
				groupSeparator(options.groupSeparator) {}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONS_HPP */
