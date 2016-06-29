#include "StackTraceIndenterBase8.hpp"

using std::string;

namespace redengine {
namespace error {

	StackTraceIndenterBase8::StackTraceIndenterBase8() : indentString("    "), marginWidth(0u), indentWidth(0u) {}

	StackTraceIndenterBase8::StackTraceIndenterBase8(const StackTraceIndenterBase8& indenter)
			: Indenter(indenter), marginString(indenter.marginString), indentString(indenter.indentString),
			marginWidth(indenter.marginWidth), indentWidth(indenter.indentWidth) {}

	StackTraceIndenterBase8::~StackTraceIndenterBase8() {}

	void StackTraceIndenterBase8::setMarginString(const string& marginString) {
		this->marginString = marginString;
	}

	unsigned StackTraceIndenterBase8::getEffectiveMarginWidth() const {
		return marginWidth ? marginWidth : static_cast<unsigned>(marginString.length());
	}

	void StackTraceIndenterBase8::setIndentString(const string& indentString) {
		this->indentString = indentString;
	}

	unsigned StackTraceIndenterBase8::getEffectiveIndentWidth() const {
		return indentWidth ? indentWidth : static_cast<unsigned>(indentString.length());
	}

	unsigned StackTraceIndenterBase8::indent(unsigned levels) {
		unsigned inherited = indentOwnInherited();
		writeString(marginString);
		unsigned u;
		for(u = 0u; u < levels; ++u)
			writeString(indentString);
		return inherited + getEffectiveMarginWidth() + levels * getEffectiveIndentWidth();
	}

	void StackTraceIndenterBase8::skip(unsigned columns) {
		string strut(static_cast<string::size_type>(columns), ' ');
		writeString(strut);
	}

}}
