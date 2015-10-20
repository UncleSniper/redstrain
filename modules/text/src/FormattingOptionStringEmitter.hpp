#ifndef REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONSSTRINGEMITTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONSSTRINGEMITTER_HPP

#include <string>

#include "FormattingStringOption.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class FormattingOptionStringEmitter {

	  public:
		typedef CharT Char;
		typedef std::basic_string<CharT> String;

	  public:
		FormattingOptionStringEmitter() {}
		FormattingOptionStringEmitter(const FormattingOptionStringEmitter&) {}
		virtual ~FormattingOptionStringEmitter() {}

		virtual String emitString(FormattingStringOption) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTINGOPTIONSSTRINGEMITTER_HPP */
