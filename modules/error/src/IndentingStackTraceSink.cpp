#include "IndentingStackTraceSink.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace error {

	// ======== Indenter ========

	IndentingStackTraceSink::Indenter::Indenter() {}

	IndentingStackTraceSink::Indenter::Indenter(const Indenter&) {}

	IndentingStackTraceSink::Indenter::~Indenter() {}

	// ======== IndentingStackTraceSink ========

	IndentingStackTraceSink::IndentingStackTraceSink(Indenter& indenter, unsigned indentLevel)
			: indenter(indenter), indentLevel(indentLevel) {}

	IndentingStackTraceSink::IndentingStackTraceSink(const IndentingStackTraceSink& sink)
			: StackTraceSink(sink), StackTraceSinkBase(sink),
			indenter(sink.indenter), indentLevel(sink.indentLevel) {}

	void IndentingStackTraceSink::beginHeader() {
		indenter.indent(indentLevel);
	}

	void IndentingStackTraceSink::endHeader() {
		indenter.endLine();
	}

	void IndentingStackTraceSink::beginFrame() {
		indenter.indent(indentLevel + 1u);
	}

	void IndentingStackTraceSink::beginFrameModule(MemorySize) {
		indenter.endLine();
		indenter.indent(indentLevel + 1u);
	}

	void IndentingStackTraceSink::endFrameModule() {}

	void IndentingStackTraceSink::endFrame() {
		indenter.endLine();
	}

}}
