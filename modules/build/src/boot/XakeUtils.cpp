#include <cctype>
#include <sstream>
#include <algorithm>

#include "XakeUtils.hpp"

using std::map;
using std::string;
using std::transform;
using std::stringstream;

namespace redengine {
namespace build {
namespace boot {

	string XakeUtils::subst(const string& strTemplate, const map<string, string>& variables) {
		stringstream ss;
		string::size_type old = static_cast<string::size_type>(0u), pos;
		while((pos = strTemplate.find('%', old)) != string::npos) {
			string::size_type end = strTemplate.find('%', pos + static_cast<string::size_type>(1u));
			if(end == string::npos)
				break;
			string var(strTemplate.substr(pos + static_cast<string::size_type>(1u),
					end - pos - static_cast<string::size_type>(1u)));
			map<string, string>::const_iterator it = variables.find(var);
			if(it == variables.end()) {
				ss << strTemplate.substr(old, end - old + static_cast<string::size_type>(1u));
				old = end;
				continue;
			}
			ss << strTemplate.substr(old, pos - old) << it->second;
			old = end + static_cast<string::size_type>(1u);
		}
		ss << strTemplate.substr(old);
		return ss.str();
	}

	static char slugify(char c) {
		if(
			(c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9')
		)
			return static_cast<char>(static_cast<unsigned char>(static_cast<unsigned>(toupper(
				static_cast<int>(static_cast<unsigned int>(static_cast<unsigned char>(c)))
			))));
		else
			return '_';
	}

	string XakeUtils::slugifyMacro(const string& macro) {
		string sm(macro);
		transform(sm.begin(), sm.end(), sm.begin(), slugify);
		return sm;
	}

}}}
