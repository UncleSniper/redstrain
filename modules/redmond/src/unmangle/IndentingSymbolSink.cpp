#include "IndentingSymbolSink.hpp"
#include "../IndentationChain.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	// ======== Indenter ========

	IndentingSymbolSink::Indenter::Indenter() {}

	IndentingSymbolSink::Indenter::Indenter(const Indenter&) {}

	IndentingSymbolSink::Indenter::~Indenter() {}

	void IndentingSymbolSink::Indenter::indentOwnInherited() const {
		if(chain)
			chain->indentInherited();
	}

	// ======== IndentingSymbolSink ========

	IndentingSymbolSink::IndentingSymbolSink(Indenter& indenter) : indenter(indenter), currentLevel(0u) {}

	IndentingSymbolSink::IndentingSymbolSink(const IndentingSymbolSink& sink)
			: SymbolSink(sink), indenter(sink.indenter), currentLevel(sink.currentLevel) {}

	void IndentingSymbolSink::notifyIndented(unsigned) {}

	void IndentingSymbolSink::startNewLine(int levelDelta) {
		if(levelDelta < 0) {
			unsigned ld = static_cast<unsigned>(-levelDelta);
			currentLevel = ld > currentLevel ? 0u : currentLevel - ld;
		}
		else
			currentLevel += static_cast<unsigned>(levelDelta);
		indenter.startNewLine();
		notifyIndented(indenter.indent(currentLevel));
	}

}}}
