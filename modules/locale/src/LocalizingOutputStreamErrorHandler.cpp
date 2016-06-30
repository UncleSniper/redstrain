#include <redstrain/error/StackTrace.hpp>
#include <redstrain/text/OutputStreamStackTraceIndenter16.hpp>

#include "LocalizingOutputStreamErrorHandler.hpp"
#include "LocalizingOutputStreamStackTraceSink.hpp"

using redengine::text::Char16;
using redengine::io::OutputStream;
using redengine::error::StackTrace;
using redengine::text::OutputStreamStackTraceIndenter16;

namespace redengine {
namespace locale {

	LocalizingOutputStreamErrorHandler::LocalizingOutputStreamErrorHandler(OutputStream<Char16>& stream,
			Indenter& indenter)
			: OutputStreamErrorHandler16(stream, indenter) {}

	LocalizingOutputStreamErrorHandler::LocalizingOutputStreamErrorHandler(OutputStream<Char16>& stream,
			Indenter& indenter, const Locale& desiredLocale)
			: OutputStreamErrorHandler16(stream, indenter), LocalizingErrorHandler(desiredLocale) {}

	LocalizingOutputStreamErrorHandler::LocalizingOutputStreamErrorHandler(OutputStream<Char16>& stream,
			Indenter& indenter, StackTraceMessageSource<Char16>& messageSource)
			: OutputStreamErrorHandler16(stream, indenter), LocalizingErrorHandler(messageSource) {}

	LocalizingOutputStreamErrorHandler::LocalizingOutputStreamErrorHandler(OutputStream<Char16>& stream,
			Indenter& indenter, StackTraceMessageSource<Char16>& messageSource, const Locale& desiredLocale)
			: OutputStreamErrorHandler16(stream, indenter), LocalizingErrorHandler(messageSource, desiredLocale) {}

	LocalizingOutputStreamErrorHandler::LocalizingOutputStreamErrorHandler(const
			LocalizingOutputStreamErrorHandler& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase16(handler), OutputStreamErrorHandler16(handler), LocalizingErrorHandler(handler) {}

	void LocalizingOutputStreamErrorHandler::writeStackTrace(const StackTrace* trace) {
		OutputStreamStackTraceIndenter16 indenter(getStream());
		indenter.setIndentationChain(this);
		LocalizingOutputStreamStackTraceSink sink(getStream(), indenter, getMessageSource(), getDesiredLocale());
		sink.advanceCurrentColumn(getCurrentColumn());
		configureStackTraceSink(sink, indenter);
		configureLocalizingStackTraceSink(sink, indenter);
		if(trace)
			trace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void LocalizingOutputStreamErrorHandler::configureLocalizingStackTraceSink(
			LocalizingOutputStreamStackTraceSink&, OutputStreamStackTraceIndenter16&) {}

}}
