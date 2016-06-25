#include <map>
#include <iostream>
#include <redstrain/util/Delete.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/ConsoleSymbolSink8.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/redmond/unmangle-gcc.hpp>

#include "Options.hpp"
#include "Unmangler.hpp"
#include "predicates.hpp"

using std::map;
using std::cerr;
using std::endl;
using std::string;
using redengine::util::Delete;
using redengine::platform::Console;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::io::ConsoleSymbolSink8;
using redengine::io::FormattedInputStream;
using redengine::io::FormattedOutputStream;
using redengine::redmond::unmangle::CPPSymbol;
using redengine::redmond::unmangleCPPSymbol_GCC3;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::cmdline::runWithOptions;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("nowrap", &Options::setAvoidWrapping);
	logic.addShortOption('w', &Options::setAvoidWrapping);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

typedef map<string, Unmangler> Schemes;
typedef Schemes::const_iterator SchemeIterator;

void registerSchemes(Schemes& schemes) {
	schemes["gcc3"] = Unmangler(unmangleCPPSymbol_GCC3, "_Z", gcc3symCharPredicate);
}

int run(const string& progname, const Options& options) {
	Schemes schemes;
	registerSchemes(schemes);
	SchemeIterator sit(schemes.find(options.getScheme()));
	if(sit == schemes.end()) {
		cerr << progname << ": Unknown mangling scheme: " << options.getScheme() << endl;
		return 1;
	}
	const Unmangler& unmangler = sit->second;
	FileOutputStream out(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	out.setCloseOnDestroy(false);
	FormattedOutputStream<char> fout(out);
	ConsoleSymbolSink8 sink(Console::STANDARD_OUTPUT);
	if(options.shouldAvoidWrapping())
		sink.setColumnCount(0u);
	Delete<CPPSymbol> symbol;
	if(options.hasSymbol()) {
		symbol = unmangler.unmanglePlainSymbol(options.getSymbol());
		if(*symbol) {
			symbol->print(sink);
			fout.endLine();
		}
		else
			fout.println(options.getSymbol());
		return 0;
	}
	FileInputStream in(Console::getStandardHandle(Console::STANDARD_INPUT));
	FormattedInputStream<char> fin(in);
	string line, unmangled;
	string::size_type llen, pos, fstart, flen;
	while(fin.readLine(line)) {
		llen = line.length();
		pos = static_cast<string::size_type>(0u);
		for(;;) {
			symbol = unmangler.filterUnmangling(line, pos, fstart, flen);
			if(!*symbol)
				break;
			if(fstart > pos)
				fout.print(line.substr(pos, fstart - pos));
			symbol->print(sink);
			delete symbol.set();
			pos = fstart + flen;
		}
		fout.println(line.substr(pos));
		sink.resetCurrentColumn();
		line.clear();
	}
	return 0;
}
