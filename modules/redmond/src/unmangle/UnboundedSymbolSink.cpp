#include "UnboundedSymbolSink.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	UnboundedSymbolSink::UnboundedSymbolSink() {}

	UnboundedSymbolSink::UnboundedSymbolSink(const UnboundedSymbolSink& sink) : SymbolSink(sink) {}

	unsigned UnboundedSymbolSink::getRemainingColumnCount() {
		return 0u;
	}

	unsigned UnboundedSymbolSink::getInlineWidthOf(const Type&, const CurrentTemplateArguments&, const Type*) {
		return 0u;
	}

	unsigned UnboundedSymbolSink::getInlineWidthOf(const Name&, const CurrentTemplateArguments&, const string*) {
		return 0u;
	}

	unsigned UnboundedSymbolSink::getInlineWidthOf(const TemplateArgument&, const CurrentTemplateArguments&) {
		return 0u;
	}

	unsigned UnboundedSymbolSink::getInlineWidthOf(Separator) {
		return 0u;
	}

	void UnboundedSymbolSink::startNewLine(int) {}

}}}
