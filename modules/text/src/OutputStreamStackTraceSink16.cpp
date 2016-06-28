#include "OutputStreamSymbolSink16.hpp"
#include "OutputStreamSymbolIndenter16.hpp"
#include "OutputStreamStackTraceSink16.hpp"

using redengine::util::MemorySize;
using redengine::io::OutputStream;
using redengine::redmond::unmangle::CPPSymbol;

namespace redengine {
namespace text {

	OutputStreamStackTraceSink16::OutputStreamStackTraceSink16(OutputStream<Char16>& stream, Indenter& indenter)
			: IndentingStackTraceSink(indenter), stream(stream), formatted(stream) {}

	OutputStreamStackTraceSink16::OutputStreamStackTraceSink16(const OutputStreamStackTraceSink16& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase16(sink), IndentingStackTraceSink(sink), stream(sink.stream), formatted(stream) {}

	OutputStreamStackTraceSink16::~OutputStreamStackTraceSink16() {}

	void OutputStreamStackTraceSink16::writeHeader(const String16& data) {
		formatted.print(data);
	}

	void OutputStreamStackTraceSink16::writeFrame(const String16& data) {
		formatted.print(data);
	}

	void OutputStreamStackTraceSink16::writeSymbol(const CPPSymbol& symbol) {
		OutputStreamSymbolIndenter16 indenter(stream);
		OutputStreamSymbolSink16 sink(stream, indenter);
		configureSymbolSink(sink, indenter);
		symbol.print(sink);
	}

	void OutputStreamStackTraceSink16::beginFrameModule(MemorySize addressLength) {
		Indenter& indenter = getIndenter();
		indenter.endLine();
		indenter.indent(getIndentLevel() + 1u);
		indenter.skip(static_cast<unsigned>(addressLength));
	}

	void OutputStreamStackTraceSink16::writeFrameModule(const String16& data) {
		formatted.print(data);
	}

	void OutputStreamStackTraceSink16::configureSymbolSink(OutputStreamSymbolSink16&,
			OutputStreamSymbolIndenter16&) {}

}}
