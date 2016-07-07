#ifndef REDSTRAIN_MOD_LOCALE_LOCALIZINGCONSOLEERRORHANDLER_HPP
#define REDSTRAIN_MOD_LOCALE_LOCALIZINGCONSOLEERRORHANDLER_HPP

#include <redstrain/text/ConsoleErrorHandler16.hpp>
#include <redstrain/text/ConsoleOutputStreamStackTraceSink16.hpp>

#include "LocalizingErrorHandler.hpp"
#include "LocalizingStackTraceSink.hpp"

namespace redengine {
namespace locale {

	class REDSTRAIN_LOCALE_API LocalizingConsoleErrorHandler
			: text::ConsoleErrorHandler16, public LocalizingErrorHandler {

	  public:
		class REDSTRAIN_LOCALE_API ChainedLocalizingConsoleStackTraceSink
				: public text::ConsoleOutputStreamStackTraceSink16, public LocalizingStackTraceSink {

		  public:
			ChainedLocalizingConsoleStackTraceSink(io::OutputStream<text::Char16>&, Indenter&, platform::Console*);
			ChainedLocalizingConsoleStackTraceSink(io::OutputStream<text::Char16>&, Indenter&, platform::Console*,
					const Locale&);
			ChainedLocalizingConsoleStackTraceSink(io::OutputStream<text::Char16>&, Indenter&, platform::Console*,
					StackTraceMessageSource<text::Char16>&);
			ChainedLocalizingConsoleStackTraceSink(io::OutputStream<text::Char16>&, Indenter&, platform::Console*,
					StackTraceMessageSource<text::Char16>&, const Locale&);
			ChainedLocalizingConsoleStackTraceSink(const ChainedLocalizingConsoleStackTraceSink&);

		};

	  protected:
		virtual void writeStackTrace(const error::StackTrace*);
		virtual void configureChainedLocalizingConsoleStackTraceSink(ChainedLocalizingConsoleStackTraceSink&,
				text::OutputStreamStackTraceIndenter16&);

	  public:
		LocalizingConsoleErrorHandler(platform::Console::StandardHandle);
		LocalizingConsoleErrorHandler(platform::Console::StandardHandle, const Locale&);
		LocalizingConsoleErrorHandler(platform::Console::StandardHandle, StackTraceMessageSource<text::Char16>&);
		LocalizingConsoleErrorHandler(platform::Console::StandardHandle,
				StackTraceMessageSource<text::Char16>&, const Locale&);
		LocalizingConsoleErrorHandler(const LocalizingConsoleErrorHandler&);

		static LocalizingConsoleErrorHandler& getDefaultStdErrorHandler();

	};

}}

#endif /* REDSTRAIN_MOD_LOCALE_LOCALIZINGCONSOLEERRORHANDLER_HPP */
