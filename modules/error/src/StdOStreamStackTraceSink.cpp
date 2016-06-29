#include <redstrain/redmond/unmangle/CPPSymbol.hpp>

#include "StdOStreamStackTraceSink.hpp"

using std::string;
using std::ostream;
using redengine::util::MemorySize;
using redengine::redmond::unmangle::CPPSymbol;
using redengine::redmond::unmangle::StdOStreamSymbolSink;
using redengine::redmond::unmangle::StdOStreamSymbolIndenter;

namespace redengine {
namespace error {

	StdOStreamStackTraceSink::StdOStreamStackTraceSink(ostream& stream, Indenter& indenter)
			: IndentingStackTraceSink(indenter), stream(stream) {}

	StdOStreamStackTraceSink::StdOStreamStackTraceSink(const StdOStreamStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), HighlightingStackTraceSink(sink),
			StackTraceSinkBase8(sink), IndentingStackTraceSink(sink), stream(sink.stream) {}

	void StdOStreamStackTraceSink::writeHeader(const string& data) {
		stream << data;
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void StdOStreamStackTraceSink::writeFrame(const string& data) {
		stream << data;
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void StdOStreamStackTraceSink::writeSymbol(const CPPSymbol& symbol) {
		StdOStreamSymbolIndenter indenter(stream);
		indenter.setIndentationChain(this);
		StdOStreamSymbolSink sink(stream, indenter);
		sink.advanceCurrentColumn(getCurrentColumn());
		configureSymbolSink(sink, indenter);
		symbol.print(sink);
	}

	void StdOStreamStackTraceSink::beginFrameModule(MemorySize addressLength) {
		Indenter& indenter = getIndenter();
		indenter.endLine();
		resetCurrentColumn();
		advanceCurrentColumn(indenter.indent(getIndentLevel() + 1u));
		indenter.skip(static_cast<unsigned>(addressLength));
		advanceCurrentColumn(static_cast<unsigned>(addressLength));
	}

	void StdOStreamStackTraceSink::writeFrameModule(const string& data) {
		stream << data;
		advanceCurrentColumn(static_cast<unsigned>(data.length()));
	}

	void StdOStreamStackTraceSink::configureSymbolSink(StdOStreamSymbolSink&, StdOStreamSymbolIndenter&) {}

}}
