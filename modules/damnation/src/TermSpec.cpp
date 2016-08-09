#include <map>
#include <string>
#include <cstdlib>
#include <iostream>

#include "termdb.hpp"

using std::map;
using std::cerr;
using std::endl;
using std::string;

namespace redengine {
namespace damnation {

	typedef map<string, const TermSpec*> TermDBCache;

	static TermDBCache* termDBCache = NULL;

	static void buildTermDBCache() {
		bool debug = !!getenv("RSDAMN_DEBUG_TERMDB_CACHE");
		termDBCache = new TermDBCache;
		const TermSpec* spec = TERM_DBASE;
		string name;
		for(; spec->names; ++spec) {
			string names(spec->names);
			string::size_type old = static_cast<string::size_type>(0u), pos;
			while((pos = names.find('|', old)) != string::npos) {
				if(pos > old) {
					name = names.substr(old, pos - old);
					(*termDBCache)[name] = spec;
					if(debug)
						cerr << "***libdamnation++: Adding terminal type '" << name << "'." << endl;
				}
				old = pos + static_cast<string::size_type>(1u);
			}
			if(old < names.length()) {
				name = names.substr(old);
				(*termDBCache)[name] = spec;
				if(debug)
					cerr << "***libdamnation++: Adding terminal type '" << name << "'." << endl;
			}
		}
	}

	REDSTRAIN_DAMNATION_API const TermSpec* getTermSpecFor(const char* termName) {
		if(!termName)
			return NULL;
		if(!termDBCache)
			buildTermDBCache();
		TermDBCache::const_iterator it = termDBCache->find(termName);
		return it == termDBCache->end() ? NULL : it->second;
	}

	REDSTRAIN_DAMNATION_API const TermSpec* getTermSpecForThisTerminal() {
		const TermSpec* type = getTermSpecFor(getenv("TERM"));
		return type ? type : getTermSpecFor("dumb");
	}

}}
