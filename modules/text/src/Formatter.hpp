#ifndef REDSTRAIN_MOD_TEXT_FORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTER_HPP

#include "Formattable.hpp"

namespace redengine {
namespace text {

	template<
		typename CharT,
		typename NumericRenditionT = DefaultFormattingRendition<CharT>
	>
	class Formatter {

	  public:
		typedef CharT Char;
		typedef NumericRenditionT NumericRendition;
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
