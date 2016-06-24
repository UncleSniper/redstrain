#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_HIGHLIGHTINGSYMBOLSINK_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_HIGHLIGHTINGSYMBOLSINK_HPP

#include "SymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API HighlightingSymbolSink : public virtual SymbolSink {

	  protected:
		virtual void highlightCVQualifier();
		virtual void highlightKeyword();
		virtual void highlightTypeModifier();
		virtual void highlightSymbolicConstant();
		virtual void highlightSourceName(const std::string&);
		virtual void highlightNumericLiteral();
		virtual void highlightDiscriminator();
		virtual void highlightUndefinedTemplateParameter();
		virtual void highlightBuiltinType();
		virtual void endHighlighting();

	  public:
		HighlightingSymbolSink();
		HighlightingSymbolSink(const HighlightingSymbolSink&);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_HIGHLIGHTINGSYMBOLSINK_HPP */
