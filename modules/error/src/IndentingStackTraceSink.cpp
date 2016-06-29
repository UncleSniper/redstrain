#include "IndentingStackTraceSink.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace error {

	// ======== Indenter ========

	IndentingStackTraceSink::Indenter::Indenter() {}

	IndentingStackTraceSink::Indenter::Indenter(const Indenter&) {}

	IndentingStackTraceSink::Indenter::~Indenter() {}

	unsigned IndentingStackTraceSink::Indenter::indentOwnInherited() const {
		return chain ? chain->indentInherited() : 0u;
	}

	// ======== IndentingStackTraceSink ========

	IndentingStackTraceSink::IndentingStackTraceSink(Indenter& indenter, unsigned indentLevel)
			: indenter(indenter), indentLevel(indentLevel), withinFrame(false), currentColumn(0u) {}

	IndentingStackTraceSink::IndentingStackTraceSink(const IndentingStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), IndentationChain(sink),
			indenter(sink.indenter), indentLevel(sink.indentLevel), withinFrame(sink.withinFrame),
			currentColumn(sink.currentColumn) {}

	void IndentingStackTraceSink::advanceCurrentColumn(unsigned columnDelta) {
		currentColumn += columnDelta;
	}

	void IndentingStackTraceSink::resetCurrentColumn() {
		currentColumn = 0u;
	}

	void IndentingStackTraceSink::beginHeader() {
		currentColumn += indenter.indent(indentLevel);
	}

	void IndentingStackTraceSink::endHeader() {
		indenter.endLine();
		currentColumn = 0u;
	}

	void IndentingStackTraceSink::beginFrame() {
		withinFrame = true;
		currentColumn += indenter.indent(indentLevel + 1u);
	}

	void IndentingStackTraceSink::beginFrameModule(MemorySize) {
		indenter.endLine();
		currentColumn = indenter.indent(indentLevel + 1u);
	}

	void IndentingStackTraceSink::endFrameModule() {}

	void IndentingStackTraceSink::endFrame() {
		indenter.endLine();
		withinFrame = false;
		currentColumn = 0u;
	}

	unsigned IndentingStackTraceSink::indentInherited() {
		unsigned columns = indenter.indent(indentLevel + static_cast<unsigned>(withinFrame));
		currentColumn += columns;
		return columns;
	}

}}
