#include <sstream>

#include "XakeUtils.hpp"

using std::map;
using std::string;
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

}}}
