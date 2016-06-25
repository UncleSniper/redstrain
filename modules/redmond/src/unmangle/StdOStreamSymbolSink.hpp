#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_STDOSTREAMSYMBOLSINK_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_STDOSTREAMSYMBOLSINK_HPP

#include <iostream>

#include "SymbolSinkBase8.hpp"
#include "BoundedSymbolSink.hpp"
#include "SymbolWidthAdapter.hpp"
#include "IndentingSymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API StdOStreamSymbolSink
			: public SymbolSinkBase8, public BoundedSymbolSink, public IndentingSymbolSink {

	  private:
		std::ostream& stream;
		bool lastWasGreater, lastWasLess;

	  protected:
		virtual void writeString(const std::string&);
		virtual void notifyIndented(unsigned);

	  public:
		StdOStreamSymbolSink(std::ostream&, Indenter&, int = 0, unsigned = 0u);
		StdOStreamSymbolSink(const StdOStreamSymbolSink&);

		inline std::ostream& getStream() {
			return stream;
		}

		inline const std::ostream& getStream() const {
			return stream;
		}

		virtual void startNewLine(int);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_STDOSTREAMSYMBOLSINK_HPP */
