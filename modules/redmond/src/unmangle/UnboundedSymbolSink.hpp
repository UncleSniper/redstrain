#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_UNBOUNDEDSYMBOLSINK_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_UNBOUNDEDSYMBOLSINK_HPP

#include "SymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API UnboundedSymbolSink : public virtual SymbolSink {

	  public:
		UnboundedSymbolSink();
		UnboundedSymbolSink(const UnboundedSymbolSink&);

		virtual unsigned getRemainingColumnCount();
		virtual unsigned getInlineWidthOf(const Type&, const CurrentTemplateArguments&, const Type*);
		virtual unsigned getInlineWidthOf(const TemplateArgument&, const CurrentTemplateArguments&);
		virtual unsigned getInlineWidthOf(Separator);
		virtual void startNewLine(int);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_UNBOUNDEDSYMBOLSINK_HPP */
