#include "LocalizingConsoleStackTraceSink.hpp"

using redengine::text::Char16;
using redengine::platform::Console;

namespace redengine {
namespace locale {

	LocalizingConsoleStackTraceSink::LocalizingConsoleStackTraceSink(Console::StandardHandle handle)
			: ConsoleStackTraceSink16(handle) {}

	LocalizingConsoleStackTraceSink::LocalizingConsoleStackTraceSink(Console::StandardHandle handle,
			const Locale& desiredLocale)
			: ConsoleStackTraceSink16(handle), LocalizingStackTraceSink(desiredLocale) {}

	LocalizingConsoleStackTraceSink::LocalizingConsoleStackTraceSink(Console::StandardHandle handle,
			StackTraceMessageSource<Char16>& messageSource)
			: ConsoleStackTraceSink16(handle), LocalizingStackTraceSink(messageSource) {}

	LocalizingConsoleStackTraceSink::LocalizingConsoleStackTraceSink(Console::StandardHandle handle,
			StackTraceMessageSource<Char16>& messageSource, const Locale& desiredLocale)
			: ConsoleStackTraceSink16(handle), LocalizingStackTraceSink(messageSource, desiredLocale) {}

	LocalizingConsoleStackTraceSink::LocalizingConsoleStackTraceSink(const LocalizingConsoleStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase16(sink), ConsoleStackTraceSink16(sink), LocalizingStackTraceSink(sink) {}

}}
