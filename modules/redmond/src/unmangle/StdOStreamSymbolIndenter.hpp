#ifndef REDSTRAIN_MOD_REDMOND_UNMANGLE_STDOSTREAMSYMBOLINDENTER_HPP
#define REDSTRAIN_MOD_REDMOND_UNMANGLE_STDOSTREAMSYMBOLINDENTER_HPP

#include <iostream>

#include "SymbolSinkIndenterBase8.hpp"

namespace redengine {
namespace redmond {
namespace unmangle {

	class REDSTRAIN_REDMOND_API StdOStreamSymbolIndenter : public SymbolSinkIndenterBase8 {

	  private:
		std::ostream& stream;

	  protected:
		virtual void writeString(const std::string&);

	  public:
		StdOStreamSymbolIndenter(std::ostream&);
		StdOStreamSymbolIndenter(const StdOStreamSymbolIndenter&);

		inline std::ostream& getStream() {
			return stream;
		}

		inline const std::ostream& getStream() const {
			return stream;
		}

		virtual void startNewLine();

	};

}}}

#endif /* REDSTRAIN_MOD_REDMOND_UNMANGLE_STDOSTREAMSYMBOLINDENTER_HPP */
