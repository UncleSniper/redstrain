#include "SymbolSinkIndenterBase16.hpp"

namespace redengine {
namespace text {

#define c16(c) static_cast<Char16>(c)

	static const Char16 DEFAULT_INDENT_STRING[] = {
		c16(' '), c16(' '), c16(' '), c16(' '), c16('\0')
	};

	const String16 SymbolSinkIndenterBase16::defaultIndentString(DEFAULT_INDENT_STRING);

	SymbolSinkIndenterBase16::SymbolSinkIndenterBase16()
			: indentString(defaultIndentString), marginWidth(0u), indentWidth(0u) {}

	SymbolSinkIndenterBase16::SymbolSinkIndenterBase16(const SymbolSinkIndenterBase16& indenter)
			: Indenter(indenter), marginString(indenter.marginString), indentString(indenter.indentString),
			marginWidth(indenter.marginWidth), indentWidth(indenter.indentWidth) {}

	SymbolSinkIndenterBase16::~SymbolSinkIndenterBase16() {}

	void SymbolSinkIndenterBase16::setMarginString(const String16& marginString) {
		this->marginString = marginString;
	}

	unsigned SymbolSinkIndenterBase16::getEffectiveMarginWidth() const {
		return marginWidth ? marginWidth : static_cast<unsigned>(marginString.length());
	}

	void SymbolSinkIndenterBase16::setIndentString(const String16& indentString) {
		this->indentString = indentString;
	}

	unsigned SymbolSinkIndenterBase16::getEffectiveIndentWidth() const {
		return indentWidth ? indentWidth : static_cast<unsigned>(indentString.length());
	}

	unsigned SymbolSinkIndenterBase16::indent(unsigned levels) {
		indentOwnInherited();
		writeString(marginString);
		unsigned u;
		for(u = 0u; u < levels; ++u)
			writeString(indentString);
		return getEffectiveMarginWidth() + levels * getEffectiveIndentWidth();
	}

}}
