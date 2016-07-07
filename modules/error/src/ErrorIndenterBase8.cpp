#include "ErrorIndenterBase8.hpp"

using std::string;

namespace redengine {
namespace error {

	const string ErrorIndenterBase8::DEFAULT_INDENT_STRING("    ");

	ErrorIndenterBase8::ErrorIndenterBase8() : indentWidth(0u) {}

	ErrorIndenterBase8::ErrorIndenterBase8(const ErrorIndenterBase8& indenter)
			: Indenter(indenter), indentString(indenter.indentString), indentWidth(indenter.indentWidth) {}

	ErrorIndenterBase8::~ErrorIndenterBase8() {}

	void ErrorIndenterBase8::setIndentString(const string& indentString) {
		this->indentString = indentString;
	}

	unsigned ErrorIndenterBase8::getEffectiveIndentWidth() const {
		return indentWidth ? indentWidth : static_cast<unsigned>(indentString.length());
	}

	unsigned ErrorIndenterBase8::indent() {
		writeString(indentString);
		return getEffectiveIndentWidth();
	}

}}
