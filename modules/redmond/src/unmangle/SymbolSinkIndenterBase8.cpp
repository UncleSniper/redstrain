#include "SymbolSinkIndenterBase8.hpp"

using std::string;

namespace redengine {
namespace redmond {
namespace unmangle {

	SymbolSinkIndenterBase8::SymbolSinkIndenterBase8() : indentString("    "), marginWidth(0u), indentWidth(0u) {}

	SymbolSinkIndenterBase8::SymbolSinkIndenterBase8(const SymbolSinkIndenterBase8& indenter)
			: Indenter(indenter), marginString(indenter.marginString), indentString(indenter.indentString),
			marginWidth(indenter.marginWidth), indentWidth(indenter.indentWidth) {}

	SymbolSinkIndenterBase8::~SymbolSinkIndenterBase8() {}

	void SymbolSinkIndenterBase8::setMarginString(const string& marginString) {
		this->marginString = marginString;
	}

	unsigned SymbolSinkIndenterBase8::getEffectiveMarginWidth() const {
		return marginWidth ? marginWidth : static_cast<unsigned>(marginString.length());
	}

	void SymbolSinkIndenterBase8::setIndentString(const string& indentString) {
		this->indentString = indentString;
	}

	unsigned SymbolSinkIndenterBase8::getEffectiveIndentWidth() const {
		return indentWidth ? indentWidth : static_cast<unsigned>(indentString.length());
	}

	unsigned SymbolSinkIndenterBase8::indent(unsigned levels) {
		writeString(marginString);
		unsigned u;
		for(u = 0u; u < levels; ++u)
			writeString(indentString);
		return getEffectiveMarginWidth() + levels * getEffectiveIndentWidth();
	}

}}}
