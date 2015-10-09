#include "CPPUtils.hpp"

using std::string;

namespace redengine {
namespace util {

	static const char *const HEX_DIGITS = "0123456789ABCDEF";

	string CPPUtils::escapeChar(char c, bool withQuotes, char quote) {
		char buf[7];
		unsigned code;
		switch(c) {
			case '\\':
				return withQuotes ? "'\\\\'" : "\\\\";
			case '\r':
				return withQuotes ? "'\\r'" : "\\r";
			case '\n':
				return withQuotes ? "'\\n'" : "\\n";
			case '\f':
				return withQuotes ? "'\\f'" : "\\f";
			case '\v':
				return withQuotes ? "'\\v'" : "\\v";
			case '\a':
				return withQuotes ? "'\\a'" : "\\a";
			case '\b':
				return withQuotes ? "'\\b'" : "\\b";
			case '\t':
				return withQuotes ? "'\\t'" : "\\t";
			default:
				if(c == quote) {
					if(withQuotes) {
						buf[0] = '\'';
						buf[1] = '\\';
						buf[2] = c;
						buf[3] = '\'';
						buf[4] = '\0';
					}
					else {
						buf[0] = '\\';
						buf[1] = c;
						buf[2] = '\0';
					}
					return buf;
				}
				if(c >= ' ' && c <= '~') {
					if(withQuotes) {
						buf[0] = '\'';
						buf[1] = c;
						buf[2] = '\'';
						buf[3] = '\0';
					}
					else {
						buf[0] = c;
						buf[1] = '\0';
					}
					return buf;
				}
				code = static_cast<unsigned>(static_cast<unsigned char>(c));
				if(withQuotes) {
					buf[0] = '\'';
					buf[1] = '\\';
					buf[2] = 'x';
					buf[3] = HEX_DIGITS[code / 16u];
					buf[4] = HEX_DIGITS[code % 16u];
					buf[5] = '\'';
					buf[6] = '\0';
				}
				else {
					buf[0] = '\\';
					buf[1] = 'x';
					buf[2] = HEX_DIGITS[code / 16u];
					buf[3] = HEX_DIGITS[code % 16u];
					buf[4] = '\0';
				}
				return buf;
		}
	}

	string CPPUtils::escapeChar(char c, bool withQuotes) {
		return CPPUtils::escapeChar(c, withQuotes, '\'');
	}

	string CPPUtils::escapeString(const string& s, bool withQuotes) {
		string r;
		r.reserve(s.length() * static_cast<string::size_type>(2u));
		if(withQuotes)
			r += '"';
		string::const_iterator begin(s.begin()), end(s.end());
		for(; begin != end; ++begin)
			r += CPPUtils::escapeChar(*begin, false, '"');
		if(withQuotes)
			r += '"';
		return r;
	}

}}
