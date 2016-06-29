#include "StackTraceIndenterBase16.hpp"

namespace redengine {
namespace text {

#define c16(c) static_cast<Char16>(c)

	static const Char16 defaultIndentString[] = {c16(' '), c16(' '), c16(' '), c16(' '), c16('\0')};

	const String16 StackTraceIndenterBase16::DEFAULT_INDENT_STRING(defaultIndentString);

	StackTraceIndenterBase16::StackTraceIndenterBase16()
			: indentString(StackTraceIndenterBase16::DEFAULT_INDENT_STRING), marginWidth(0u), indentWidth(0u) {}

	StackTraceIndenterBase16::StackTraceIndenterBase16(const StackTraceIndenterBase16& indenter)
			: Indenter(indenter), marginString(indenter.marginString), indentString(indenter.indentString),
			marginWidth(indenter.marginWidth), indentWidth(indenter.indentWidth) {}

	StackTraceIndenterBase16::~StackTraceIndenterBase16() {}

	void StackTraceIndenterBase16::setMarginString(const String16& marginString) {
		this->marginString = marginString;
	}

	unsigned StackTraceIndenterBase16::getEffectiveMarginWidth() const {
		return marginWidth ? marginWidth : static_cast<unsigned>(marginString.length());
	}

	void StackTraceIndenterBase16::setIndentString(const String16& indentString) {
		this->indentString = indentString;
	}

	unsigned StackTraceIndenterBase16::getEffectiveIndentWidth() const {
		return indentWidth ? indentWidth : static_cast<unsigned>(indentString.length());
	}

	unsigned StackTraceIndenterBase16::indent(unsigned levels) {
		unsigned inherited = indentOwnInherited();
		writeString(marginString);
		unsigned u;
		for(u = 0u; u < levels; ++u)
			writeString(indentString);
		return inherited + getEffectiveMarginWidth() + levels * getEffectiveIndentWidth();
	}

	void StackTraceIndenterBase16::skip(unsigned columns) {
		String16 strut(static_cast<String16::size_type>(columns), c16(' '));
		writeString(strut);
	}

}}
