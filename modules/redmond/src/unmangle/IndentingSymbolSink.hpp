#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_INDENTINGSYMBOLSINK_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_INDENTINGSYMBOLSINK_HPP

#include "SymbolSink.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API IndentingSymbolSink : public virtual SymbolSink {

	  public:
		class REDSTRAIN_REDMOND_API Indenter {

		  public:
			Indenter();
			Indenter(const Indenter&);
			virtual ~Indenter();

			virtual void startNewLine() = 0;
			virtual unsigned indent(unsigned) = 0;

		};

	  private:
		Indenter& indenter;
		unsigned currentLevel;

	  protected:
		virtual void notifyIndented(unsigned);

	  public:
		IndentingSymbolSink(Indenter&);
		IndentingSymbolSink(const IndentingSymbolSink&);

		inline Indenter& getIndenter() {
			return indenter;
		}

		inline const Indenter& getIndenter() const {
			return indenter;
		}

		virtual void startNewLine(int);

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_INDENTINGSYMBOLSINK_HPP */
