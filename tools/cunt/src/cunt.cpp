#include <map>
#include <set>
#include <list>
#include <cstring>
#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/damnation/TermSpec.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/streamtypes.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Options.hpp"
#include "blobdepend.hpp"

using std::map;
using std::set;
using std::cerr;
using std::endl;
using std::list;
using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::platform::Console;
using redengine::damnation::TSFlag;
using redengine::damnation::TermSpec;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::io::FormattedInputStream;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::io::operator<<;
using redengine::cmdline::runWithOptions;
using redengine::io::endln;
using redengine::damnation::TS_FLAG_DIMENSION;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run,
			APPROPRIATE_CONSOLE_ERROR_HANDLER);
}

typedef void (*IntCapability)(TermSpec&, uint32_t);
typedef void (*StringCapability)(TermSpec&, const char*);

map<string, TSFlag> knownBoolCaps;
map<string, IntCapability> knownIntCaps;
map<string, StringCapability> knownStringCaps;

void initKnownBoolCaps() {
	#define def(code, bit) knownBoolCaps[code] = redengine::damnation::bit;
	#define ignore(code) knownBoolCaps[code] = redengine::damnation::TSF__LAST;
	def("bw", TSF_AUTO_LEFT_MARGIN)
	def("am", TSF_AUTO_RIGHT_MARGIN)
	def("ut", TSF_BACK_COLOR_ERASE)
	def("cc", TSF_CAN_CHANGE)
	def("xs", TSF_CEOL_STANDOUT_GLITCH)
	def("YA", TSF_COL_ADDR_GLITCH)
	def("YF", TSF_CPI_CHANGES_RES)
	def("YB", TSF_CR_CANCELS_MICRO_MODE)
	def("xt", TSF_DEST_TABS_MAGIC_SMSO)
	def("xn", TSF_EAT_NEWLINE_GLITCH)
	def("eo", TSF_ERASE_OVERSTRIKE)
	def("gn", TSF_GENERIC_TYPE)
	def("hc", TSF_HARD_COPY)
	def("HC", TSF_HARD_CURSOR)
	def("km", TSF_HAS_META_KEY)
	def("YC", TSF_HAS_PRINT_WHEEL)
	def("hs", TSF_HAS_STATUS_LINE)
	def("hl", TSF_HUE_LIGHNESS_SATURATION)
	def("in", TSF_INSERT_NULL_GLITCH)
	def("YG", TSF_LPI_CHANGES_RES)
	def("da", TSF_MEMORY_ABOVE)
	def("db", TSF_MEMORY_BELOW)
	def("mi", TSF_MOVE_INSERT_MODE)
	def("ms", TSF_MOVE_STANDOUT_MODE)
	def("nx", TSF_NEEDS_XON_XOFF)
	def("xb", TSF_NO_ESC_CTLC)
	def("NP", TSF_NO_PAD_CHAR)
	def("ND", TSF_NON_DEST_SCROLL_REGION)
	def("NR", TSF_NON_REV_RMCUP)
	def("os", TSF_OVER_STRIKE)
	def("5i", TSF_PRTR_SILENT)
	def("YD", TSF_ROW_ADDR_GLITCH)
	def("YE", TSF_SEMI_AUTO_RIGHT_MARGIN)
	def("es", TSF_STATUS_LINE_ESC_OK)
	def("hz", TSF_TILDE_GLITCH)
	def("ul", TSF_TRANSPARENT_UNDERLINE)
	def("xo", TSF_XON_XOFF)
	def("AX", TSF_SCREEN_UNDERSTANDS_ANSI_SET_DEFAULT_COLOR)
	def("XT", TSF_SCREEN_UNDERSTANDS_XTERM_SEQUENCES)
	def("G0", TSF_SCREEN_UNDERSTANDS_ISO2022)
	// undocumented in terminfo(5)
	def("bs", TSF_DPRCT_BACKSPACE_MOVE_LEFT)
	def("pt", TSF_DPRCT_PERFORMS_HARDWARE_TABS)
	def("nc", TSF_DPRCT_DO_NOT_USE_ASCII_CR)
	def("ns", TSF_DOES_NOT_SCROLL_FOR_SEQUENTIAL_OUTPUT)
	// Nobody seems to know what 'xs@' is, perhaps the same as 'xs'?
	// Or maybe it's leftover from the 'import exception' mechanism...
	def("xs@", TSF_CEOL_STANDOUT_GLITCH)
	def("xo@", TSF_XON_XOFF)  // ditto
	def("ms@", TSF_MOVE_STANDOUT_MODE)  // ditto
	def("bw@", TSF_AUTO_LEFT_MARGIN)  // ditto
	def("bs@", TSF_DPRCT_BACKSPACE_MOVE_LEFT)  // ditto
	def("xn@", TSF_EAT_NEWLINE_GLITCH)  // ditto
	def("ut@", TSF_BACK_COLOR_ERASE)  // ditto
	def("cc@", TSF_CAN_CHANGE)  // ditto
	def("hz@", TSF_TILDE_GLITCH)  // ditto
	def("hs@", TSF_HAS_STATUS_LINE)  // ditto
	def("xb@", TSF_NO_ESC_CTLC)  // ditto
	// *Really* nobody knows just what the <kinsoku jikou>
	// 'NL' is supposed to be.
	ignore("NL")
	ignore("le@")  // ditto
	ignore("mh@")  // ditto
	ignore("mk@")  // ditto
	ignore("sf@")  // ditto
	ignore("sg@")  // ditto
	ignore("%c@")  // ditto
	ignore("%e@")  // ditto
	ignore("NC@")  // ditto
	ignore("Sb@")  // ditto
	ignore("Sf@")  // ditto
	ignore("ch@")  // ditto
	ignore("cv@")  // ditto
	ignore("%0@")  // ditto
	ignore("%1@")  // ditto
	ignore("*6@")  // ditto
	ignore("@0@")  // ditto
	ignore("F3@")  // ditto
	ignore("F4@")  // ditto
	ignore("F5@")  // ditto
	ignore("F6@")  // ditto
	ignore("F7@")  // ditto
	ignore("F8@")  // ditto
	ignore("F9@")  // ditto
	ignore("FA@")  // ditto
	ignore("l1@")  // ditto
	ignore("l2@")  // ditto
	ignore("l3@")  // ditto
	ignore("l4@")  // ditto
	ignore("vb@")  // ditto
	ignore("ue@")  // ditto
	ignore("us@")  // ditto
	ignore("mb@")  // ditto
	ignore("md@")  // ditto
	ignore("sr@")  // ditto
	ignore("SF@")  // ditto
	ignore("SR@")  // ditto
	ignore("AB@")  // ditto
	ignore("AF@")  // ditto
	ignore("Co@")  // ditto
	ignore("op@")  // ditto
	ignore("pa@")  // ditto
	ignore("ei@")  // ditto
	ignore("im@")  // ditto
	ignore("pf@")  // ditto
	ignore("po@")  // ditto
	ignore("ps@")  // ditto
	ignore("ic@")  // ditto
	ignore("k1@")  // ditto
	ignore("k2@")  // ditto
	ignore("k3@")  // ditto
	ignore("k4@")  // ditto
	ignore("k5@")  // ditto
	ignore("k6@")  // ditto
	ignore("k7@")  // ditto
	ignore("k8@")  // ditto
	ignore("dm@")  // ditto
	ignore("ed@")  // ditto
	ignore("kC@")  // ditto
	ignore("kh@")  // ditto
	ignore("RA@")  // ditto
	ignore("SA@")  // ditto
	ignore("rc@")  // ditto
	ignore("sc@")  // ditto
	ignore("F1@")  // ditto
	ignore("F2@")  // ditto
	ignore("k9@")  // ditto
	ignore("k;@")  // ditto
	ignore("FB@")  // ditto
	ignore("FC@")  // ditto
	ignore("FD@")  // ditto
	ignore("FE@")  // ditto
	ignore("FF@")  // ditto
	ignore("FG@")  // ditto
	ignore("FH@")  // ditto
	ignore("FI@")  // ditto
	ignore("FJ@")  // ditto
	ignore("FK@")  // ditto
	ignore("FL@")  // ditto
	ignore("FM@")  // ditto
	ignore("FN@")  // ditto
	ignore("FO@")  // ditto
	ignore("FP@")  // ditto
	ignore("FQ@")  // ditto
	ignore("FR@")  // ditto
	ignore("FS@")  // ditto
	ignore("FT@")  // ditto
	ignore("FU@")  // ditto
	ignore("FV@")  // ditto
	ignore("FW@")  // ditto
	ignore("FX@")  // ditto
	ignore("FY@")  // ditto
	ignore("FZ@")  // ditto
	ignore("Fa@")  // ditto
	ignore("ae@")  // ditto
	ignore("as@")  // ditto
	ignore("i3@")  // ditto
	ignore("ve@")  // ditto
	ignore("vs@")  // ditto
	ignore("pb@")  // ditto
	ignore("ip@")  // ditto
	ignore("am@")  // ditto
	ignore("ho@")  // ditto
	ignore("ll@")  // ditto
	ignore("km@")  // ditto
	ignore("5i@")  // ditto
	ignore("AL@")  // ditto
	ignore("DL@")  // ditto
	ignore("al@")  // ditto
	ignore("dl@")  // ditto
	ignore("eA@")  // ditto
	ignore("is@")  // ditto
	ignore("r1@")  // ditto
	ignore("do@")  // ditto
	ignore("up@")  // ditto
	ignore("k0@")  // ditto
	ignore("l0@")  // ditto
	ignore("cm@")  // ditto
	ignore("se@")  // ditto
	ignore("so@")  // ditto
	ignore("dc@")  // ditto
	ignore("ct@")  // ditto
	ignore("it@")  // ditto
	ignore("ta@")  // ditto
	ignore("ac@")  // ditto
	ignore("ke@")  // ditto
	ignore("ks@")  // ditto
	ignore("kd@")  // ditto
	ignore("kl@")  // ditto
	ignore("kr@")  // ditto
	ignore("ku@")  // ditto
	ignore("Ic@")  // ditto
	ignore("cs@")  // ditto
	ignore("kb@")  // ditto
	ignore("te@")  // ditto
	ignore("ti@")  // ditto
	ignore("Ip@")  // ditto
	ignore("kB@")  // ditto
	ignore("oc@")  // ditto
	ignore("vi@")  // ditto
	ignore("#4@")  // ditto
	ignore("%i@")  // ditto
	ignore("kF@")  // ditto
	ignore("kR@")  // ditto
	ignore("bl@")  // ditto
	ignore("IC@")  // ditto
	ignore("S2@")  // ditto
	ignore("S3@")  // ditto
	ignore("sa@")  // ditto
	ignore("rp@")  // ditto
	ignore("u6@")  // ditto
	ignore("u7@")  // ditto
	ignore("l5@")  // ditto
	ignore("rf@")  // ditto
	ignore("@8@")  // ditto
	ignore("K1@")  // ditto
	ignore("K2@")  // ditto
	ignore("K3@")  // ditto
	ignore("K4@")  // ditto
	ignore("K5@")  // ditto
	ignore("ds@")  // ditto
	ignore("fs@")  // ditto
	ignore("ts@")  // ditto
	ignore("ws@")  // ditto
	ignore("i1@")  // ditto
	ignore("cd@")  // ditto
	ignore("ce@")  // ditto
	ignore("!3@")  // ditto
	ignore("%h@")  // ditto
	ignore("%j@")  // ditto
	ignore("&7@")  // ditto
	ignore("sp@")  // ditto
	ignore("ZH@")  // ditto
	ignore("ZR@")  // ditto
	ignore("ec@")  // ditto
	ignore("#3@")  // ditto
	ignore("ml@")  // ditto
	ignore("mu@")  // ditto
	ignore("dN@")  // ditto
	ignore("vt@")  // ditto
	ignore("cb@")  // ditto
	ignore("Ms@")  // ditto
	ignore("E3@")  // ditto
	ignore("me@")  // ditto
	ignore("mr@")  // ditto
	ignore("S6@")  // ditto
	ignore("Nl@")  // ditto
	ignore("lh@")  // ditto
	ignore("lw@")  // ditto
	ignore("pn@")  // ditto
	ignore("ma@")  // ditto
	ignore("nw@")  // ditto
	ignore("@1@")  // ditto
	#undef def
	#undef ignore
}

#define def(code, name) \
	void setIntCap_ ## name(TermSpec& spec, uint32_t value) { \
		spec.name = value; \
	} \
	uint32_t getIntCap_ ## name(const TermSpec& spec) { \
		return spec.name; \
	}
#define ignore(code)
#include "intcaps.hpp"
#undef def
#undef ignore

void ignoreIntCap(TermSpec&, uint32_t) {}

void initKnownIntCaps() {
	#define def(code, name) knownIntCaps[code] = setIntCap_ ## name;
	#define ignore(code) knownIntCaps[code] = ignoreIntCap;
	#include "intcaps.hpp"
	#undef def
	#undef ignore
}

#define def(code, name) \
	void setStringCap_ ## name(TermSpec& spec, const char* value) { \
		spec.name = value; \
	} \
	const char* getStringCap_ ## name(const TermSpec& spec) { \
		return spec.name; \
	}
#define alias(code, name)
#define ignore(code)
#include "strcaps.hpp"
#undef def
#undef alias
#undef ignore

void ignoreStringCap(TermSpec&, const char*) {}

void initKnownStringCaps() {
	#define def(code, name) knownStringCaps[code] = setStringCap_ ## name;
	#define alias(code, name) def(code, name)
	#define ignore(code) knownStringCaps[code] = ignoreStringCap;
	#include "strcaps.hpp"
	#undef def
	#undef alias
	#undef ignore
}

struct Entry;

map<string, Entry*> entries;

struct Entry {

	list<string> names;
	string description;
	set<string> boolCaps;
	map<string, uint32_t> intCaps;
	map<string, string> stringCaps;
	list<string> pendingImports;
	string nameList;

	void insert() {
		if(names.empty()) {
			delete this;
			return;
		}
		if(names.size() > static_cast<list<string>::size_type>(1u)) {
			description = names.back();
			names.pop_back();
		}
		list<string>::const_iterator begin(names.begin()), end(names.end());
		for(; begin != end; ++begin)
			entries[*begin] = this;
	}

	void import(const Entry& other) {
		set<string>::const_iterator bbegin(other.boolCaps.begin()), bend(other.boolCaps.end());
		for(; bbegin != bend; ++bbegin)
			boolCaps.insert(*bbegin);
		map<string, uint32_t>::const_iterator ibegin(other.intCaps.begin()), iend(other.intCaps.end());
		for(; ibegin != iend; ++ibegin) {
			if(intCaps.find(ibegin->first) == intCaps.end())
				intCaps.insert(*ibegin);
		}
		map<string, string>::const_iterator sbegin(other.stringCaps.begin()), send(other.stringCaps.end());
		for(; sbegin != send; ++sbegin) {
			if(stringCaps.find(sbegin->first) == stringCaps.end())
				stringCaps.insert(*sbegin);
		}
	}

	int importAll() {
		list<string>::const_iterator begin(pendingImports.begin()), end(pendingImports.end());
		for(; begin != end; ++begin) {
			map<string, Entry*>::const_iterator it = entries.find(*begin);
			if(it == entries.end()) {
				cerr << "Entry '" << names.front() << "' refers to undefined entry '" << *begin << "'" << endl;
				return 1;
			}
			int status = it->second->importAll();
			if(status)
				return status;
			import(*it->second);
		}
		pendingImports.clear();
		return 0;
	}

	int toSpec(TermSpec& spec) {
		if(nameList.empty()) {
			list<string>::const_iterator nbegin(names.begin()), nend(names.end());
			for(; nbegin != nend; ++nbegin) {
				if(!nameList.empty())
					nameList += '|';
				nameList += *nbegin;
			}
		}
		memset(&spec, 0, sizeof(TermSpec));
		spec.names = nameList.c_str();
		if(!description.empty())
			spec.description = description.c_str();
		set<string>::const_iterator bbegin(boolCaps.begin()), bend(boolCaps.end());
		for(; bbegin != bend; ++bbegin) {
			map<string, TSFlag>::const_iterator it = knownBoolCaps.find(*bbegin);
			if(it == knownBoolCaps.end()) {
				cerr << "Unrecognized boolean capability '" << *bbegin
						<< "' for terminal '" << nameList << "'" << endl;
				return 1;
			}
			if(it->second == redengine::damnation::TSF__LAST)
				continue;
			unsigned bit = static_cast<unsigned>(it->second);
			spec.flags[bit / 8u] |= static_cast<uint32_t>(1u) << (bit % 8u);
		}
		map<string, uint32_t>::const_iterator ibegin(intCaps.begin()), iend(intCaps.end());
		for(; ibegin != iend; ++ibegin) {
			map<string, IntCapability>::const_iterator it = knownIntCaps.find(ibegin->first);
			if(it == knownIntCaps.end()) {
				cerr << "Unrecognized integer capability '" << ibegin->first
						<< "' for terminal '" << nameList << "'" << endl;
				return 1;
			}
			it->second(spec, ibegin->second);
		}
		map<string, string>::const_iterator sbegin(stringCaps.begin()), send(stringCaps.end());
		for(; sbegin != send; ++sbegin) {
			map<string, StringCapability>::const_iterator it = knownStringCaps.find(sbegin->first);
			if(it == knownStringCaps.end()) {
				cerr << "Unrecognized string capability '" << sbegin->first
						<< "' for terminal '" << nameList << "'" << endl;
				return 1;
			}
			it->second(spec, sbegin->second.c_str());
		}
		return 0;
	}

};

int parse(FormattedInputStream<char>& instream) {
	string line, name, strvalue;
	uint32_t ivalue;
	enum {
		ST_NONE,
		ST_NONE_ESCAPE,
		ST_COMMENT,
		ST_TERMNAME,
		ST_TERMNAME_ESCAPE,
		ST_TERMNAME_BEGIN,
		ST_CAPNAME,
		ST_CAPNAME_ESCAPE,
		ST_CAPNAME_BEGIN,
		ST_INTVALUE,
		ST_INTZERO,
		ST_INTOCTAL,
		ST_INTEMPTYHEX,
		ST_INTHEX,
		ST_INTDECIMAL,
		ST_STRINGVALUE,
		ST_STRINGBEGIN,
		ST_STRINGCARET,
		ST_STRINGESC,
		ST_STRINGESCZERO,
		ST_STRINGESCOCT1,
		ST_STRINGESCOCT2,
		ST_STRINGESCOCT3
	} state = ST_NONE;
	unsigned lno = 1u;
	Entry* entry = new Entry;
	for(;;) {
		line.clear();
		if(!instream.readLine(line))
			break;
		string::const_iterator begin(line.begin()), end(line.end());
		for(; begin != end; ++begin) {
			char c = *begin;
		  again:
			switch(state) {
				case ST_NONE:
					switch(c) {
						case '|':
							name.clear();
							state = ST_TERMNAME;
							break;
						case ':':
							cerr << "Empty terminal name in line " << lno << endl;
							return 1;
						case '\\':
							state = ST_NONE_ESCAPE;
							break;
						case '\t':
							break;
						case '#':
							state = ST_COMMENT;
							break;
						default:
							name.assign(&c, static_cast<string::size_type>(1u));
							state = ST_TERMNAME;
							break;
					}
					break;
				case ST_NONE_ESCAPE:
				case ST_TERMNAME_ESCAPE:
					cerr << "Escape in terminal name in line " << lno << endl;
					return 1;
				case ST_COMMENT:
					break;
				case ST_TERMNAME_BEGIN:
					if(c == '\t')
						break;
					state = ST_TERMNAME;
				case ST_TERMNAME:
					switch(c) {
						case '|':
							if(!name.empty()) {
								entry->names.push_back(name);
								name.clear();
							}
							break;
						case ':':
							if(!name.empty()) {
								entry->names.push_back(name);
								name.clear();
							}
							if(entry->names.empty()) {
								cerr << "Entry without a name in line " << lno << endl;
								return 1;
							}
							state = ST_CAPNAME;
							break;
						case '\\':
							state = ST_TERMNAME_ESCAPE;
							break;
						default:
							name += c;
							break;
					}
					break;
				case ST_CAPNAME_ESCAPE:
					cerr << "Escape in capability name in line " << lno << endl;
					return 1;
				case ST_CAPNAME_BEGIN:
					if(c == '\t')
						break;
					state = ST_CAPNAME;
				case ST_CAPNAME:
					switch(c) {
						case ':':
							if(!name.empty()) {
								if(name[0] != '.')
									entry->boolCaps.insert(name);
								name.clear();
							}
							break;
						case '\\':
							state = ST_CAPNAME_ESCAPE;
							break;
						case '#':
							if(name.empty()) {
								name += c;
								break;
							}
							ivalue = static_cast<uint32_t>(0u);
							state = ST_INTVALUE;
							break;
						case '=':
							if(name.empty()) {
								cerr << "String capability with empty name in line " << lno << endl;
								return 1;
							}
							strvalue.clear();
							state = ST_STRINGVALUE;
							break;
						default:
							name += c;
							break;
					}
					break;
				case ST_INTVALUE:
					if(c == '0')
						state = ST_INTZERO;
					else if(c >= '1' && c <= '9') {
						ivalue = static_cast<uint32_t>(c - '0');
						state = ST_INTDECIMAL;
					}
					else {
						cerr << "Illegal character within integer in line " << lno << endl;
						return 1;
					}
					break;
				case ST_INTZERO:
					if(c == 'x') {
						state = ST_INTEMPTYHEX;
						break;
					}
					ivalue = static_cast<uint32_t>(0u);
					state = ST_INTOCTAL;
				case ST_INTOCTAL:
					if(c == ':') {
						if(name[0] != '.')
							entry->intCaps[name] = static_cast<uint32_t>(0u);
						name.clear();
						ivalue = static_cast<uint32_t>(0u);
						state = ST_CAPNAME;
					}
					else if(c >= '0' && c <= '7')
						ivalue = ivalue * static_cast<uint32_t>(8u) + static_cast<uint32_t>(c - '0');
					else {
						cerr << "Illegal digit in octal integer capability in line " << lno << endl;
						return 1;
					}
					break;
				case ST_INTEMPTYHEX:
				case ST_INTHEX:
					if(c == ':') {
						if(state == ST_INTEMPTYHEX) {
							cerr << "Hexadecimal integer capability without digits in line " << lno << endl;
							return 1;
						}
						if(name[0] != '.')
							entry->intCaps[name] = ivalue;
						name.clear();
						ivalue = static_cast<uint32_t>(0u);
						state = ST_CAPNAME;
					}
					else if(c >= '0' && c <= '9') {
						ivalue = ivalue * static_cast<uint32_t>(16u) * static_cast<uint32_t>(c - '0');
						state = ST_INTHEX;
					}
					else if(c >= 'a' && c <= 'f') {
						ivalue = ivalue * static_cast<uint32_t>(16u) * static_cast<uint32_t>(c - 'a' + 10);
						state = ST_INTHEX;
					}
					else if(c >= 'A' && c <= 'F') {
						ivalue = ivalue * static_cast<uint32_t>(16u) * static_cast<uint32_t>(c - 'A' + 10);
						state = ST_INTHEX;
					}
					else {
						cerr << "Illegal digit in hexadecimal integer capability in line " << lno << endl;
						return 1;
					}
					break;
				case ST_INTDECIMAL:
					if(c == ':') {
						if(name[0] != '.')
							entry->intCaps[name] = ivalue;
						name.clear();
						ivalue = static_cast<uint32_t>(0u);
						state = ST_CAPNAME;
					}
					else if(c >= '0' && c <= '9')
						ivalue = ivalue * static_cast<uint32_t>(10u) * static_cast<uint32_t>(c - '0');
					else {
						cerr << "Illegal digit in decimal integer capability in line " << lno << endl;
						return 1;
					}
					break;
				case ST_STRINGBEGIN:
					if(c == '\t')
						break;
					state = ST_STRINGVALUE;
				case ST_STRINGVALUE:
					switch(c) {
						case ':':
							if(name == "tc")
								entry->pendingImports.push_back(strvalue);
							else
								entry->stringCaps[name] = strvalue;
							name.clear();
							strvalue.clear();
							state = ST_CAPNAME;
							break;
						case '^':
							state = ST_STRINGCARET;
							break;
						case '\\':
							state = ST_STRINGESC;
							break;
						default:
							strvalue += c;
							break;
					}
					break;
				case ST_STRINGCARET:
					{
						unsigned code = static_cast<unsigned>(static_cast<unsigned char>(c)) & ~64u;
						strvalue += static_cast<char>(static_cast<unsigned char>(code));
						state = ST_STRINGVALUE;
					}
					break;
				case ST_STRINGESC:
					state = ST_STRINGVALUE;
					switch(c) {
						#define clamp(rendition, binary) \
							case rendition: \
								strvalue += binary; \
								break;
						clamp('e', '\033')
						clamp('E', '\033')
						clamp('n', '\n')
						clamp('l', '\n')
						clamp('r', '\r')
						clamp('t', '\t')
						clamp('b', '\b')
						clamp('f', '\f')
						clamp('s', ' ')
						clamp('^', '^')
						clamp('\\', '\\')
						clamp(':', ':')
						#undef clamp
						case '0':
							state = ST_STRINGESCZERO;
							break;
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
							ivalue = static_cast<uint32_t>(c - '0');
							state = ST_STRINGESCOCT1;
							break;
					}
					break;
				case ST_STRINGESCZERO:
					if(c >= '0' && c <= '7') {
						ivalue = static_cast<uint32_t>(c - '0');
						state = ST_STRINGESCOCT2;
					}
					else {
						strvalue += '\200';
						state = ST_STRINGVALUE;
						goto again;
					}
					break;
				case ST_STRINGESCOCT1:
				case ST_STRINGESCOCT2:
					if(c >= '0' && c <= '7') {
						ivalue = ivalue * static_cast<uint32_t>(8u) + static_cast<uint32_t>(c - '0');
						if(state == ST_STRINGESCOCT1)
							state = ST_STRINGESCOCT2;
						else {
							strvalue += static_cast<char>(static_cast<unsigned char>(ivalue));
							state = ST_STRINGVALUE;
						}
					}
					else {
						cerr << "Illegal octal digit '" << c << "' within escape sequence in "
								"string capability '" << name << "' in line " << lno << endl;
						return 1;
					}
					break;
				case ST_STRINGESCOCT3:
					cerr << "Programmer fsck(8)ed up: ST_STRINGESCOCT3 should never be reached" << endl;
					return 1;
				default:
					cerr << "Programmer fsck(8)ed up: Unrecognized state: " << static_cast<int>(state) << endl;
					return 1;
			}
		}
		switch(state) {
			case ST_NONE:
				break;
			case ST_NONE_ESCAPE:
			case ST_COMMENT:
				state = ST_NONE;
				break;
			case ST_TERMNAME:
			case ST_TERMNAME_BEGIN:
				cerr << "Bare terminal name line given as entry in line " << lno << endl;
				return 1;
			case ST_TERMNAME_ESCAPE:
				state = ST_TERMNAME_BEGIN;
				break;
			case ST_CAPNAME:
			case ST_CAPNAME_BEGIN:
				if(!name.empty()) {
					if(name[0] != '.')
						entry->boolCaps.insert(name);
					name.clear();
				}
				entry->insert();
				entry = new Entry;
				state = ST_NONE;
				break;
			case ST_CAPNAME_ESCAPE:
				state = ST_CAPNAME_BEGIN;
				break;
			case ST_INTVALUE:
				cerr << "Empty integer capability in line " << lno << endl;
				return 1;
			case ST_INTZERO:
			case ST_INTOCTAL:
			case ST_INTDECIMAL:
			case ST_INTHEX:
				cerr << "Line " << lno << " ends within integer capability" << endl;
				return 1;
			case ST_INTEMPTYHEX:
				cerr << "Hexadecimal integer capability without digits in line " << lno << endl;
				return 1;
			case ST_STRINGVALUE:
			case ST_STRINGBEGIN:
			case ST_STRINGCARET:
				cerr << "Line " << lno << " ends within string capability" << endl;
				return 1;
			case ST_STRINGESC:
			case ST_STRINGESCZERO:
			case ST_STRINGESCOCT1:
			case ST_STRINGESCOCT2:
			case ST_STRINGESCOCT3:
				cerr << "Line " << lno << " ends within escape sequence in string capability" << endl;
				return 1;
			default:
				cerr << "Programmer fsck(8)ed up: Unrecognized state: " << static_cast<int>(state) << endl;
				return 1;
		}
		++lno;
	}
	switch(state) {
		case ST_NONE:
		case ST_NONE_ESCAPE:
		case ST_COMMENT:
		case ST_TERMNAME:
		case ST_TERMNAME_ESCAPE:
			break;
		case ST_TERMNAME_BEGIN:
			cerr << "Bare terminal name line given as entry in line " << lno << endl;
			return 1;
		case ST_CAPNAME:
		case ST_CAPNAME_ESCAPE:
		case ST_CAPNAME_BEGIN:
			if(!name.empty()) {
				if(name[0] != '.')
					entry->boolCaps.insert(name);
				name.clear();
			}
			entry->insert();
			entry = new Entry;
			break;
		case ST_INTVALUE:
			cerr << "Empty integer capability in line " << lno << endl;
			return 1;
		case ST_INTZERO:
		case ST_INTOCTAL:
		case ST_INTHEX:
		case ST_INTDECIMAL:
			cerr << "Line " << lno << " ends within integer capability" << endl;
			return 1;
		case ST_INTEMPTYHEX:
			cerr << "Hexadecimal integer capability without digits in line " << lno << endl;
			return 1;
		case ST_STRINGVALUE:
		case ST_STRINGBEGIN:
		case ST_STRINGCARET:
			cerr << "Line " << lno << " ends within string capability" << endl;
			return 1;
		case ST_STRINGESC:
		case ST_STRINGESCZERO:
		case ST_STRINGESCOCT1:
		case ST_STRINGESCOCT2:
		case ST_STRINGESCOCT3:
			cerr << "Line " << lno << " ends within escape sequence in string capability" << endl;
			return 1;
		default:
			cerr << "Programmer fsck(8)ed up: Unrecognized state: " << static_cast<int>(state) << endl;
			return 1;
	}
	entry->insert();
	map<string, Entry*>::const_iterator ebegin(entries.begin()), eend(entries.end());
	for(; ebegin != eend; ++ebegin) {
		int status = ebegin->second->importAll();
		if(status)
			return status;
	}
	return 0;
}

const char *const HEX_DIGITS = "0123456789ABCDEF";

bool isHexDigit(char c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

void dumpString(DefaultConfiguredOutputStream<char>::Stream& stream, const char* key, const char* value) {
	stream << "\t\t\t/* ." << key << " = */ ";
	if(value) {
		stream << '"';
		for(; *value; ++value) {
			switch(*value) {
				#define clamp(binary, rendition) \
					case binary: \
						stream << rendition; \
						break;
				clamp('\n', "\\n")
				clamp('\r', "\\r")
				clamp('\b', "\\b")
				clamp('\t', "\\t")
				clamp('\a', "\\a")
				clamp('\f', "\\f")
				clamp('\v', "\\v")
				clamp('"', "\\\"")
				clamp('\\', "\\\\")
				#undef clamp
				default:
					if(*value >= ' ' && *value <= '~')
						stream << *value;
					else {
						unsigned code = static_cast<unsigned>(static_cast<unsigned char>(*value));
						stream << "\\x" << HEX_DIGITS[code / 16u] << HEX_DIGITS[code % 16u];
						if(isHexDigit(value[1]))
							stream << "\"\"";
					}
					break;
			}
		}
		stream << '"';
	}
	else
		stream << "NULL";
	stream << ',' << endln;
}

void dumpFlags(DefaultConfiguredOutputStream<char>::Stream& stream, const uint32_t* flags) {
	stream << "\t\t\t/* .flags = */ {";
	unsigned u;
	for(u = 0u; u < TS_FLAG_DIMENSION; ++u) {
		if(u)
			stream << ", ";
		stream << "static_cast<uint32_t>(" << flags[u] << "ul)";
	}
	stream << "}," << endln;
}

void dumpInt(DefaultConfiguredOutputStream<char>::Stream& stream, const char* key, uint32_t value) {
	stream << "\t\t\t/* ." << key << " = */ static_cast<uint32_t>(" << value << "ul)," << endln;
}

DefaultConfiguredOutputStream<char>::Stream&
operator<<(DefaultConfiguredOutputStream<char>::Stream& stream, const TermSpec& spec) {
	stream << '{' << endln;
	#define strcap(field) dumpString(stream, #field, spec.field);
	#define intcap(field) dumpInt(stream, #field, spec.field);
	strcap(names)
	strcap(description)
	dumpFlags(stream, spec.flags);
	intcap(columns)
	intcap(init_tabs)
	intcap(label_height)
	intcap(label_width)
	intcap(lines)
	intcap(lines_of_memory)
	intcap(magic_cookie_glitch)
	intcap(max_attributes)
	intcap(max_colors)
	intcap(max_pairs)
	intcap(maximum_windows)
	intcap(no_color_video)
	intcap(num_labels)
	intcap(padding_baud_rate)
	intcap(virtual_terminal)
	intcap(width_status_line)
	intcap(underline_magic_cookie_width)
	intcap(number_of_function_keys)
	strcap(acs_chars)
	strcap(back_tab)
	strcap(bell)
	strcap(carriage_return)
	strcap(change_char_pitch)
	strcap(change_line_pitch)
	strcap(change_res_horz)
	strcap(change_res_vert)
	strcap(change_scroll_region)
	strcap(char_padding)
	strcap(clear_all_tabs)
	strcap(clear_margins)
	strcap(clear_screen)
	strcap(clr_bol)
	strcap(clr_eol)
	strcap(clr_eos)
	strcap(column_address)
	strcap(command_character)
	strcap(create_window)
	strcap(cursor_address)
	strcap(cursor_down)
	strcap(cursor_home)
	strcap(cursor_invisible)
	strcap(cursor_left)
	strcap(cursor_mem_address)
	strcap(cursor_normal)
	strcap(cursor_right)
	strcap(cursor_to_ll)
	strcap(cursor_up)
	strcap(cursor_visible)
	strcap(define_char)
	strcap(delete_character)
	strcap(delete_line)
	strcap(dial_phone)
	strcap(dis_status_line)
	strcap(display_clock)
	strcap(down_half_line)
	strcap(ena_acs)
	strcap(enter_alt_charset_mode)
	strcap(enter_am_mode)
	strcap(enter_blink_mode)
	strcap(enter_bold_mode)
	strcap(enter_ca_mode)
	strcap(enter_delete_mode)
	strcap(enter_dim_mode)
	strcap(enter_doublewide_mode)
	strcap(enter_draft_quality)
	strcap(enter_insert_mode)
	strcap(enter_italics_mode)
	strcap(enter_leftward_mode)
	strcap(enter_micro_mode)
	strcap(enter_near_letter_quality)
	strcap(enter_normal_quality)
	strcap(enter_protected_mode)
	strcap(enter_reverse_mode)
	strcap(enter_secure_mode)
	strcap(enter_shadow_mode)
	strcap(enter_standout_mode)
	strcap(enter_subscript_mode)
	strcap(enter_superscript_mode)
	strcap(enter_underline_mode)
	strcap(enter_upward_mode)
	strcap(enter_xon_mode)
	strcap(erase_chars)
	strcap(exit_alt_charset_mode)
	strcap(exit_am_mode)
	strcap(exit_attribute_mode)
	strcap(exit_ca_mode)
	strcap(exit_delete_mode)
	strcap(exit_doublewide_mode)
	strcap(exit_insert_mode)
	strcap(exit_italics_mode)
	strcap(exit_leftward_mode)
	strcap(exit_micro_mode)
	strcap(exit_shadow_mode)
	strcap(exit_standout_mode)
	strcap(exit_subscript_mode)
	strcap(exit_superscript_mode)
	strcap(exit_underline_mode)
	strcap(exit_upward_mode)
	strcap(exit_xon_mode)
	strcap(fixed_pause)
	strcap(flash_hook)
	strcap(flash_screen)
	strcap(form_feed)
	strcap(from_status_line)
	strcap(goto_window)
	strcap(hangup)
	strcap(init_1string)
	strcap(init_2string)
	strcap(init_3string)
	strcap(init_file)
	strcap(init_prog)
	strcap(initialize_color)
	strcap(initialize_pair)
	strcap(insert_character)
	strcap(insert_line)
	strcap(insert_padding)
	strcap(key_a1)
	strcap(key_a3)
	strcap(key_b2)
	strcap(key_backspace)
	strcap(key_beg)
	strcap(key_btab)
	strcap(key_c1)
	strcap(key_c3)
	strcap(key_cancel)
	strcap(key_catab)
	strcap(key_clear)
	strcap(key_close)
	strcap(key_command)
	strcap(key_copy)
	strcap(key_create)
	strcap(key_ctab)
	strcap(key_dc)
	strcap(key_dl)
	strcap(key_down)
	strcap(key_eic)
	strcap(key_end)
	strcap(key_enter)
	strcap(key_eol)
	strcap(key_eos)
	strcap(key_exit)
	strcap(key_f0)
	strcap(key_f1)
	strcap(key_f10)
	strcap(key_f11)
	strcap(key_f12)
	strcap(key_f13)
	strcap(key_f14)
	strcap(key_f15)
	strcap(key_f16)
	strcap(key_f17)
	strcap(key_f18)
	strcap(key_f19)
	strcap(key_f2)
	strcap(key_f20)
	strcap(key_f21)
	strcap(key_f22)
	strcap(key_f23)
	strcap(key_f24)
	strcap(key_f25)
	strcap(key_f26)
	strcap(key_f27)
	strcap(key_f28)
	strcap(key_f29)
	strcap(key_f3)
	strcap(key_f30)
	strcap(key_f31)
	strcap(key_f32)
	strcap(key_f33)
	strcap(key_f34)
	strcap(key_f35)
	strcap(key_f36)
	strcap(key_f37)
	strcap(key_f38)
	strcap(key_f39)
	strcap(key_f4)
	strcap(key_f40)
	strcap(key_f41)
	strcap(key_f42)
	strcap(key_f43)
	strcap(key_f44)
	strcap(key_f45)
	strcap(key_f46)
	strcap(key_f47)
	strcap(key_f48)
	strcap(key_f49)
	strcap(key_f5)
	strcap(key_f50)
	strcap(key_f51)
	strcap(key_f52)
	strcap(key_f53)
	strcap(key_f54)
	strcap(key_f55)
	strcap(key_f56)
	strcap(key_f57)
	strcap(key_f58)
	strcap(key_f59)
	strcap(key_f6)
	strcap(key_f60)
	strcap(key_f61)
	strcap(key_f62)
	strcap(key_f63)
	strcap(key_f7)
	strcap(key_f8)
	strcap(key_f9)
	strcap(key_find)
	strcap(key_help)
	strcap(key_home)
	strcap(key_ic)
	strcap(key_il)
	strcap(key_left)
	strcap(key_ll)
	strcap(key_mark)
	strcap(key_message)
	strcap(key_move)
	strcap(key_next)
	strcap(key_npage)
	strcap(key_open)
	strcap(key_options)
	strcap(key_ppage)
	strcap(key_previous)
	strcap(key_print)
	strcap(key_redo)
	strcap(key_reference)
	strcap(key_refresh)
	strcap(key_replace)
	strcap(key_restart)
	strcap(key_resume)
	strcap(key_right)
	strcap(key_save)
	strcap(key_sbeg)
	strcap(key_scancel)
	strcap(key_scommand)
	strcap(key_scopy)
	strcap(key_screate)
	strcap(key_sdc)
	strcap(key_sdl)
	strcap(key_select)
	strcap(key_send)
	strcap(key_seol)
	strcap(key_sexit)
	strcap(key_sf)
	strcap(key_sfind)
	strcap(key_shelp)
	strcap(key_shome)
	strcap(key_sic)
	strcap(key_sleft)
	strcap(key_smessage)
	strcap(key_smove)
	strcap(key_snext)
	strcap(key_soptions)
	strcap(key_sprevious)
	strcap(key_sprint)
	strcap(key_sr)
	strcap(key_sredo)
	strcap(key_sreplace)
	strcap(key_sright)
	strcap(key_srsume)
	strcap(key_ssave)
	strcap(key_ssuspend)
	strcap(key_stab)
	strcap(key_sundo)
	strcap(key_suspend)
	strcap(key_undo)
	strcap(key_up)
	strcap(keypad_local)
	strcap(keypad_xmit)
	strcap(lab_f0)
	strcap(lab_f1)
	strcap(lab_f10)
	strcap(lab_f2)
	strcap(lab_f3)
	strcap(lab_f4)
	strcap(lab_f5)
	strcap(lab_f6)
	strcap(lab_f7)
	strcap(lab_f8)
	strcap(lab_f9)
	strcap(label_format)
	strcap(label_off)
	strcap(label_on)
	strcap(meta_off)
	strcap(meta_on)
	strcap(micro_column_address)
	strcap(micro_down)
	strcap(micro_left)
	strcap(micro_right)
	strcap(micro_row_address)
	strcap(micro_up)
	strcap(newline)
	strcap(order_of_pins)
	strcap(orig_colors)
	strcap(orig_pair)
	strcap(pad_char)
	strcap(parm_dch)
	strcap(parm_delete_line)
	strcap(parm_down_cursor)
	strcap(parm_down_micro)
	strcap(parm_ich)
	strcap(parm_index)
	strcap(parm_insert_line)
	strcap(parm_left_cursor)
	strcap(parm_left_micro)
	strcap(parm_right_cursor)
	strcap(parm_right_micro)
	strcap(parm_rindex)
	strcap(parm_up_cursor)
	strcap(parm_up_micro)
	strcap(pkey_key)
	strcap(pkey_local)
	strcap(pkey_xmit)
	strcap(plab_norm)
	strcap(print_screen)
	strcap(prtr_non)
	strcap(prtr_off)
	strcap(prtr_on)
	strcap(pulse)
	strcap(quick_dial)
	strcap(remove_clock)
	strcap(repeat_char)
	strcap(req_for_input)
	strcap(reset_1string)
	strcap(reset_2string)
	strcap(reset_3string)
	strcap(reset_file)
	strcap(restore_cursor)
	strcap(row_address)
	strcap(save_cursor)
	strcap(scroll_forward)
	strcap(scroll_reverse)
	strcap(select_char_set)
	strcap(set_attributes)
	strcap(set_background)
	strcap(set_bottom_margin)
	strcap(set_bottom_margin_parm)
	strcap(set_clock)
	strcap(set_color_pair)
	strcap(set_foreground)
	strcap(set_left_margin)
	strcap(set_left_margin_parm)
	strcap(set_right_margin)
	strcap(set_right_margin_parm)
	strcap(set_tab)
	strcap(set_top_margin)
	strcap(set_top_margin_parm)
	strcap(set_window)
	strcap(start_bit_image)
	strcap(start_char_set_def)
	strcap(stop_bit_image)
	strcap(stop_char_set_def)
	strcap(subscript_characters)
	strcap(superscript_characters)
	strcap(tab)
	strcap(these_cause_cr)
	strcap(to_status_line)
	strcap(tone)
	strcap(underline_char)
	strcap(up_half_line)
	strcap(user0)
	strcap(user1)
	strcap(user2)
	strcap(user3)
	strcap(user4)
	strcap(user5)
	strcap(user6)
	strcap(user7)
	strcap(user8)
	strcap(user9)
	strcap(wait_tone)
	strcap(xoff_character)
	strcap(xon_character)
	strcap(zero_motion)
	strcap(set_a_background)
	strcap(set_a_foreground)
	strcap(reset_string)
	strcap(cursor_down_or_scroll)
	strcap(keypad_map)
	strcap(screen_switch_charset)
	strcap(screen_reset_charset)
	strcap(get_mouse)
	strcap(req_mouse_pos)
	#undef strcap
	#undef intcap
	stream << "\t\t}";
	return stream;
}

const char *const HEADER_LINES[] = {
	"/* This file was generated automatically by the 'redcunt' compiler; DO NOT EDIT it manually.",
	" * The contents herein were generated from the terminal type descriptions source file (terminfo.src)",
	" * maintained by Thomas E. Dickey (http://invisible-island.net/datafiles/current/termcap.src.gz).",
	" *",
	" * As is stated in the header of that file, there is no proper copyright/licensing for it.",
	" * As the original header by Eric S. Raymond (retained in terminfo.src) states, any attempt to",
	" * lay credible claim to the file would be outright insanity and so I trust that none of you",
	" * lunatics will try to sue me over this...",
	" *",
	" * Further, the revised header states that \"annotations [...] reflect creative effort\" and",
	" * \"the structure of entries to reuse common chunks also is creative\" and argues that there",
	" * is thus implicit copyrightery going on. Since this file is just the \"flattened\" data,",
	" * neither the annotations (i.e. comments) from the original nor the \"reuse structure\" are",
	" * present and thus those copyright claims should not apply to this version of the database.",
	" */",
	"",
	"#include \"TermSpec.hpp\"",
	"",
	NULL
};

int run(const string&, const Options& options) {
	initKnownBoolCaps();
	initKnownIntCaps();
	initKnownStringCaps();
	string infile(options.getInputFile()), outfile(options.getOutputFile());
	Delete<InputStream<char> > in;
	StreamCloser inCloser;
	if(infile == "-")
		in = new FileInputStream(Console::getStandardHandle(Console::STANDARD_INPUT));
	else {
		in = new FileInputStream(infile);
		inCloser = *in;
	}
	FormattedInputStream<char> fin(**in);
	int status = parse(fin);
	inCloser.close();
	if(status)
		return status;
	Delete<OutputStream<char> > out;
	StreamCloser outCloser;
	if(outfile == "-")
		out = new FileOutputStream(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	else {
		out = new FileOutputStream(outfile);
		outCloser = *out;
	}
	DefaultConfiguredOutputStream<char>::Stream fout(**out);
	const char *const* header = HEADER_LINES;
	for(; *header; ++header)
		fout << *header << endln;
	map<string, Entry*>::const_iterator ebegin(entries.begin()), eend(entries.end());
	TermSpec spec;
	fout << "namespace redengine {" << endln;
	fout << "namespace damnation {" << endln;
	fout << endln << "\textern const TermSpec TERM_DBASE[] = {";
	for(; ebegin != eend; ++ebegin) {
		if(ebegin->second->names.front().find('+') != string::npos)
			continue;
		status = ebegin->second->toSpec(spec);
		if(status)
			return status;
		fout << endln << "\t\t" << spec << ',';
	}
	memset(&spec, 0, sizeof(TermSpec));
	fout << endln << "\t\t" << spec << endln << "\t};" << endln << endln << "}}" << endln;
	outCloser.close();
	return 0;
}
