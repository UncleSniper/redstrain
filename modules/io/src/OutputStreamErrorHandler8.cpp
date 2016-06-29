#include <redstrain/error/StackTrace.hpp>

#include "OutputStreamErrorHandler8.hpp"
#include "OutputStreamStackTraceSink8.hpp"
#include "OutputStreamStackTraceIndenter8.hpp"

using std::string;
using redengine::error::StackTrace;

namespace redengine {
namespace io {

	OutputStreamErrorHandler8::OutputStreamErrorHandler8(OutputStream<char>& stream, Indenter& indenter)
			: IndentingErrorHandler(indenter), stream(stream), formatted(stream) {}

	OutputStreamErrorHandler8::OutputStreamErrorHandler8(const OutputStreamErrorHandler8& handler)
			: ErrorHandler(handler), ErrorHandlerTextBase(handler), HighlightingErrorHandler(handler),
			ErrorHandlerBase8(handler), IndentingErrorHandler(handler), stream(handler.stream), formatted(stream) {}

	OutputStreamErrorHandler8::~OutputStreamErrorHandler8() {}

	void OutputStreamErrorHandler8::writeString(const string& data) {
		formatted.print(data);
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void OutputStreamErrorHandler8::writeStackTrace(const StackTrace* trace) {
		OutputStreamStackTraceIndenter8 indenter(stream);
		OutputStreamStackTraceSink8 sink(stream, indenter);
		sink.advanceCurrentColumn(getCurrentColumn());
		configureStackTraceSink(sink, indenter);
		if(trace)
			trace->printTo(sink);
		else
			sink.noStackTraceAvailable();
	}

	void OutputStreamErrorHandler8::configureStackTraceSink(OutputStreamStackTraceSink8&,
			OutputStreamStackTraceIndenter8&) {}

}}
