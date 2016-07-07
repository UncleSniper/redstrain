#include <redstrain/util/Delete.hpp>
#include <redstrain/error/StackTrace.hpp>
#include <redstrain/platform/SynchronizedSingleton.hpp>
#include <redstrain/text/OutputStreamStackTraceIndenter16.hpp>

#include "LocalizingConsoleErrorHandler.hpp"

using redengine::util::Delete;
using redengine::text::Char16;
using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::error::StackTrace;
using redengine::platform::SynchronizedSingleton;
using redengine::text::OutputStreamStackTraceIndenter16;

namespace redengine {
namespace locale {

	// ======== ChainedLocalizingConsoleStackTraceSink ========

	LocalizingConsoleErrorHandler::ChainedLocalizingConsoleStackTraceSink
			::ChainedLocalizingConsoleStackTraceSink(OutputStream<Char16>& stream, Indenter& indenter,
			Console* console)
			: ConsoleOutputStreamStackTraceSink16(stream, indenter, console) {}

	LocalizingConsoleErrorHandler::ChainedLocalizingConsoleStackTraceSink
			::ChainedLocalizingConsoleStackTraceSink(OutputStream<Char16>& stream, Indenter& indenter,
			Console* console, const Locale& desiredLocale)
			: ConsoleOutputStreamStackTraceSink16(stream, indenter, console),
			LocalizingStackTraceSink(desiredLocale) {}

	LocalizingConsoleErrorHandler::ChainedLocalizingConsoleStackTraceSink
			::ChainedLocalizingConsoleStackTraceSink(OutputStream<Char16>& stream, Indenter& indenter,
			Console* console, StackTraceMessageSource<Char16>& messageSource)
			: ConsoleOutputStreamStackTraceSink16(stream, indenter, console),
			LocalizingStackTraceSink(messageSource) {}

	LocalizingConsoleErrorHandler::ChainedLocalizingConsoleStackTraceSink
			::ChainedLocalizingConsoleStackTraceSink(OutputStream<Char16>& stream, Indenter& indenter,
			Console* console, StackTraceMessageSource<Char16>& messageSource, const Locale& desiredLocale)
			: ConsoleOutputStreamStackTraceSink16(stream, indenter, console),
			LocalizingStackTraceSink(messageSource, desiredLocale) {}

	LocalizingConsoleErrorHandler::ChainedLocalizingConsoleStackTraceSink
			::ChainedLocalizingConsoleStackTraceSink(const ChainedLocalizingConsoleStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase16(sink), ConsoleOutputStreamStackTraceSink16(sink), LocalizingStackTraceSink(sink) {}

	// ======== LocalizingConsoleErrorHandler ========

	LocalizingConsoleErrorHandler::LocalizingConsoleErrorHandler(Console::StandardHandle handle)
			: ConsoleErrorHandler16(handle) {}

	LocalizingConsoleErrorHandler::LocalizingConsoleErrorHandler(Console::StandardHandle handle,
			const Locale& desiredLocale)
			: ConsoleErrorHandler16(handle), LocalizingErrorHandler(desiredLocale) {}

	LocalizingConsoleErrorHandler::LocalizingConsoleErrorHandler(Console::StandardHandle handle,
			StackTraceMessageSource<Char16>& messageSource)
			: ConsoleErrorHandler16(handle), LocalizingErrorHandler(messageSource) {}

	LocalizingConsoleErrorHandler::LocalizingConsoleErrorHandler(Console::StandardHandle handle,
			StackTraceMessageSource<Char16>& messageSource, const Locale& desiredLocale)
			: ConsoleErrorHandler16(handle), LocalizingErrorHandler(messageSource, desiredLocale) {}

	LocalizingConsoleErrorHandler::LocalizingConsoleErrorHandler(const LocalizingConsoleErrorHandler& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase16(handler), ConsoleErrorHandler16(handler), LocalizingErrorHandler(handler) {}

	void LocalizingConsoleErrorHandler::writeStackTrace(const StackTrace* trace) {
		OutputStreamStackTraceIndenter16 indenter(getStream());
		indenter.setIndentationChain(this);
		ChainedLocalizingConsoleStackTraceSink sink(getStream(), indenter,
				&getConsole(), getMessageSource(), getDesiredLocale());
		sink.advanceCurrentColumn(getCurrentColumn());
		configureStackTraceSink(sink, indenter);
		configureConsoleStackTraceSink(sink, indenter);
		configureChainedLocalizingConsoleStackTraceSink(sink, indenter);
		if(trace)
			trace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void LocalizingConsoleErrorHandler::configureChainedLocalizingConsoleStackTraceSink(
			ChainedLocalizingConsoleStackTraceSink&, OutputStreamStackTraceIndenter16&) {}

	class DefaultLocalizingConsoleErrorHandlerSingleton
			: public SynchronizedSingleton<LocalizingConsoleErrorHandler> {

	  private:
		const Console::StandardHandle handle;

	  protected:
		virtual LocalizingConsoleErrorHandler* newInstance() {
			Delete<LocalizingConsoleErrorHandler> handler(new LocalizingConsoleErrorHandler(handle));
			handler->setConsole(&handler->getConsole());   // trigger notifyConsoleChanged() to update columnCount
			return handler.set();
		}

	  public:
		DefaultLocalizingConsoleErrorHandlerSingleton(Console::StandardHandle handle) : handle(handle) {}

		DefaultLocalizingConsoleErrorHandlerSingleton(const DefaultLocalizingConsoleErrorHandlerSingleton& singleton)
				: SynchronizedSingleton<LocalizingConsoleErrorHandler>(singleton), handle(singleton.handle) {}

	};

	static DefaultLocalizingConsoleErrorHandlerSingleton stdErrSingleton(Console::STANDARD_ERROR);

	LocalizingConsoleErrorHandler& LocalizingConsoleErrorHandler::getDefaultStdErrorHandler() {
		return stdErrSingleton.get();
	}

}}
