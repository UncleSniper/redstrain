#ifndef REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGESOURCE_HPP
#define REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGESOURCE_HPP

#include <redstrain/text/types.hpp>

#include "StackTraceMessageKey.hpp"

namespace redengine {
namespace locale {

	class Locale;

	template<typename CharT>
	class StackTraceMessageSource {

	  public:
		StackTraceMessageSource() {}
		StackTraceMessageSource(const StackTraceMessageSource&) {}
		virtual ~StackTraceMessageSource() {}

		virtual std::basic_string<text::Char16> localizeMessage(const Locale&, StackTraceMessageKey) = 0;

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_STACKTRACEMESSAGESOURCE_HPP */
