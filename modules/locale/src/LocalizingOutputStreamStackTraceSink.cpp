#include "LocalizingOutputStreamStackTraceSink.hpp"

using redengine::text::Char16;
using redengine::io::OutputStream;

namespace redengine {
namespace locale {

	LocalizingOutputStreamStackTraceSink::LocalizingOutputStreamStackTraceSink(OutputStream<Char16>& stream,
			Indenter& indenter) : OutputStreamStackTraceSink16(stream, indenter) {}

	LocalizingOutputStreamStackTraceSink::LocalizingOutputStreamStackTraceSink(OutputStream<Char16>& stream,
			Indenter& indenter, const Locale& desiredLocale) : OutputStreamStackTraceSink16(stream, indenter),
			LocalizingStackTraceSink(desiredLocale) {}

	LocalizingOutputStreamStackTraceSink::LocalizingOutputStreamStackTraceSink(OutputStream<Char16>& stream,
			Indenter& indenter, StackTraceMessageSource<Char16>& messageSource)
			: OutputStreamStackTraceSink16(stream, indenter), LocalizingStackTraceSink(messageSource) {}

	LocalizingOutputStreamStackTraceSink::LocalizingOutputStreamStackTraceSink(OutputStream<Char16>& stream,
			Indenter& indenter, StackTraceMessageSource<Char16>& messageSource, const Locale& desiredLocale)
			: OutputStreamStackTraceSink16(stream, indenter),
			LocalizingStackTraceSink(messageSource, desiredLocale) {}

	LocalizingOutputStreamStackTraceSink::LocalizingOutputStreamStackTraceSink(const
			LocalizingOutputStreamStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase16(sink), OutputStreamStackTraceSink16(sink), LocalizingStackTraceSink(sink) {}

}}
