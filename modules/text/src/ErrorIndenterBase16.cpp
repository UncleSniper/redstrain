#include "ErrorIndenterBase16.hpp"

namespace redengine {
namespace text {

	const String16 ErrorIndenterBase16::DEFAULT_INDENT_STRING(static_cast<String16::size_type>(4u),
			static_cast<Char16>(' '));

	ErrorIndenterBase16::ErrorIndenterBase16() : indentWidth(0u) {}

	ErrorIndenterBase16::ErrorIndenterBase16(const ErrorIndenterBase16& indenter)
			: Indenter(indenter), indentString(indenter.indentString), indentWidth(indenter.indentWidth) {}

	ErrorIndenterBase16::~ErrorIndenterBase16() {}

	void ErrorIndenterBase16::setIndentString(const String16& indentString) {
		this->indentString = indentString;
	}

	unsigned ErrorIndenterBase16::getEffectiveIndentWidth() const {
		return indentWidth ? indentWidth : static_cast<unsigned>(indentString.length());
	}

	unsigned ErrorIndenterBase16::indent() {
		writeString(indentString);
		return getEffectiveIndentWidth();
	}

}}
