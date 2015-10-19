#ifndef REDSTRAIN_MOD_TEXT_FORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTER_HPP

#include "Formattable.hpp"
#include "Grisu2FloatFormatter.hpp"
#include "DefaultIntegerFormatter.hpp"

namespace redengine {
namespace text {

	template<
		typename CharT,
		typename NumericRenditionT = DefaultFormattingRendition<CharT>,
		typename IntegerFormatterT = DefaultIntegerFormatter<CharT>,
		typename FloatFormatterT = Grisu2FloatFormatter<CharT>
	>
	class Formatter {

	  public:
		typedef CharT Char;
		typedef NumericRenditionT NumericRendition;
		typedef IntegerFormatterT IntegerFormatter;
		typedef FloatFormatterT FloatFormatter;
		typedef std::basic_string<CharT> String;
		typedef FormattingOptions<CharT, NumericRenditionT> Options;
		typedef Formattable<CharT> Item;

	  private:
		Options defaultOptions;

	  public:
		Formatter() {}
		Formatter(const Formatter& formatter) : defaultOptions(formatter.defaultOptions) {}

		inline Options& getDefaultFormattingOptions() {
			return defaultOptions;
		}

		inline const Options& getDefaultFormattingOptions() const {
			return defaultOptions;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTER_HPP */
