#include <redstrain/error/StackTrace.hpp>

#include "OutputStreamErrorHandler16.hpp"
#include "OutputStreamStackTraceSink16.hpp"
#include "OutputStreamStackTraceIndenter16.hpp"

using redengine::io::OutputStream;
using redengine::error::StackTrace;

namespace redengine {
namespace text {

	OutputStreamErrorHandler16::OutputStreamErrorHandler16(OutputStream<Char16>& stream, Indenter& indenter)
			: IndentingErrorHandler(indenter), stream(stream), formatted(stream) {}

	OutputStreamErrorHandler16::OutputStreamErrorHandler16(const OutputStreamErrorHandler16& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase16(handler), IndentingErrorHandler(handler), stream(handler.stream), formatted(stream) {}

	OutputStreamErrorHandler16::~OutputStreamErrorHandler16() {}

	void OutputStreamErrorHandler16::writeString(const String16& data) {
		formatted.print(data);
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void OutputStreamErrorHandler16::writeStackTrace(const StackTrace* trace) {
		OutputStreamStackTraceIndenter16 indenter(stream);
		OutputStreamStackTraceSink16 sink(stream, indenter);
		sink.advanceCurrentColumn(getCurrentColumn());
		configureStackTraceSink(sink, indenter);
		if(trace)
			trace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void OutputStreamErrorHandler16::configureStackTraceSink(OutputStreamStackTraceSink16&,
			OutputStreamStackTraceIndenter16&) {}

}}
