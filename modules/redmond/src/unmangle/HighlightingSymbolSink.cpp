#include "HighlightingSymbolSink.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	HighlightingSymbolSink::HighlightingSymbolSink() {}

	HighlightingSymbolSink::HighlightingSymbolSink(const HighlightingSymbolSink& sink) : SymbolSink(sink) {}

	void HighlightingSymbolSink::highlightCVQualifier() {}

	void HighlightingSymbolSink::highlightKeyword() {}

	void HighlightingSymbolSink::highlightTypeModifier() {}

	void HighlightingSymbolSink::highlightSymbolicConstant() {}

	void HighlightingSymbolSink::highlightSourceName(const string&) {}

	void HighlightingSymbolSink::highlightNumericLiteral() {}

	void HighlightingSymbolSink::highlightDiscriminator() {}

	void HighlightingSymbolSink::highlightUndefinedTemplateParameter() {}

	void HighlightingSymbolSink::highlightBuiltinType() {}

	void HighlightingSymbolSink::endHighlighting() {}

}}}
