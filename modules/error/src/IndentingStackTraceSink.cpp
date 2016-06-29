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
			: indenter(indenter), indentLevel(indentLevel), withinFrame(false) {}

	IndentingStackTraceSink::IndentingStackTraceSink(const IndentingStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink), IndentationChain(sink),
			indenter(sink.indenter), indentLevel(sink.indentLevel), withinFrame(sink.withinFrame) {}

	void IndentingStackTraceSink::beginHeader() {
		indenter.indent(indentLevel);
	}

	void IndentingStackTraceSink::endHeader() {
		indenter.endLine();
	}

	void IndentingStackTraceSink::beginFrame() {
		withinFrame = true;
		indenter.indent(indentLevel + 1u);
	}

	void IndentingStackTraceSink::beginFrameModule(MemorySize) {
		indenter.endLine();
		indenter.indent(indentLevel + 1u);
	}

	void IndentingStackTraceSink::endFrameModule() {}

	void IndentingStackTraceSink::endFrame() {
		indenter.endLine();
		withinFrame = false;
	}

	unsigned IndentingStackTraceSink::indentInherited() {
		return indenter.indent(indentLevel + static_cast<unsigned>(withinFrame));
	}

}}
