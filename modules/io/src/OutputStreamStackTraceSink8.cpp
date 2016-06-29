#include "OutputStreamSymbolSink8.hpp"
#include "OutputStreamSymbolIndenter8.hpp"
#include "OutputStreamStackTraceSink8.hpp"

using std::string;
using redengine::util::MemorySize;
using redengine::redmond::unmangle::CPPSymbol;

namespace redengine {
namespace io {

	OutputStreamStackTraceSink8::OutputStreamStackTraceSink8(OutputStream<char>& stream, Indenter& indenter)
			: IndentingStackTraceSink(indenter), stream(stream), formatted(stream) {}

	OutputStreamStackTraceSink8::OutputStreamStackTraceSink8(const OutputStreamStackTraceSink8& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase8(sink), IndentingStackTraceSink(sink), stream(sink.stream), formatted(stream) {}

	OutputStreamStackTraceSink8::~OutputStreamStackTraceSink8() {}

	void OutputStreamStackTraceSink8::writeHeader(const string& data) {
		formatted.print(data);
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void OutputStreamStackTraceSink8::writeFrame(const string& data) {
		formatted.print(data);
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void OutputStreamStackTraceSink8::writeSymbol(const CPPSymbol& symbol) {
		OutputStreamSymbolIndenter8 indenter(stream);
		indenter.setIndentationChain(this);
		OutputStreamSymbolSink8 sink(stream, indenter);
		sink.advanceCurrentColumn(getCurrentColumn());
		configureSymbolSink(sink, indenter);
		symbol.print(sink);
	}

	void OutputStreamStackTraceSink8::beginFrameModule(MemorySize addressLength) {
		Indenter& indenter = getIndenter();
		indenter.endLine();
		resetCurrentColumn();
		advanceCurrentColumn(indenter.indent(getIndentLevel() + 1u));
		indenter.skip(static_cast<unsigned>(addressLength));
		advanceCurrentColumn(static_cast<unsigned>(addressLength));
	}

	void OutputStreamStackTraceSink8::writeFrameModule(const string& data) {
		formatted.print(data);
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void OutputStreamStackTraceSink8::configureSymbolSink(OutputStreamSymbolSink8&, OutputStreamSymbolIndenter8&) {}

}}
