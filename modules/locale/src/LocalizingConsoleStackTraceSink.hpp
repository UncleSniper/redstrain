#ifndef REDSTRAIN_MOD_LOCALE_LOCALIZINGCONSOLESTACKTRACESINK_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALIZINGCONSOLESTACKTRACESINK_HPP

#include <redstrain/text/ConsoleStackTraceSink16.hpp>

#include "LocalizingStackTraceSink.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API LocalizingConsoleStackTraceSink
			: public text::ConsoleStackTraceSink16, public LocalizingStackTraceSink {

	  public:
		LocalizingConsoleStackTraceSink(platform::Console::StandardHandle);
		LocalizingConsoleStackTraceSink(platform::Console::StandardHandle, const Locale&);
		LocalizingConsoleStackTraceSink(platform::Console::StandardHandle, StackTraceMessageSource<text::Char16>&);
		LocalizingConsoleStackTraceSink(platform::Console::StandardHandle,
				StackTraceMessageSource<text::Char16>&, const Locale&);
		LocalizingConsoleStackTraceSink(const LocalizingConsoleStackTraceSink&);

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALIZINGCONSOLESTACKTRACESINK_HPP */
