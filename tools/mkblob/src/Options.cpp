#include <iostream>
#include <redstrain/cmdline/StopExecution.hpp>
#include <redstrain/io/CPPArrayOutputStream.hpp>
#include <redstrain/cmdline/UnexpectedBarewordError.hpp>

#include "Options.hpp"

using std::cout;
using std::endl;
using std::string;
using redengine::cmdline::StopExecution;
using redengine::io::CPPArrayOutputStream;
using redengine::cmdline::UnexpectedBarewordError;

Options::Options(const Options& options) : barewordCount(options.barewordCount), progname(options.progname),
		infile(options.infile), outfile(options.outfile), varname(options.varname), expmacro(options.expmacro),
		include(options.include), header(options.header) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), infile("-"), outfile("-"),
		varname(CPPArrayOutputStream::DEFAULT_VARIABLE_NAME), header(false) {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [input-file [output-file]]" <<  endl
		<< "Options:" << endl
		<< "    --variable NAME    name the generated constant NAME instead of the default" << endl
		<< "                       of '" << CPPArrayOutputStream::DEFAULT_VARIABLE_NAME << '\'' << endl
		<< "    -n NAME            same as --variable" << endl
		<< "    --export NAME      prefix declarations/definitions with the export macro" << endl
		<< "                       NAME (required by some compilers)" << endl
		<< "    -e NAME            same as --export" << endl
		<< "    --include FILE     add #include directive for FILE" << endl
		<< "    -i FILE            same as --include" << endl
		<< "    --header           generate header instead of source" << endl
		<< "                       (input file will not be read)" << endl
		<< "    -h                 same as --header" << endl
		<< "    --blob PATH        generate blob injector into file PATH in source" << endl
		<< "    -b PATH            same as --blob" << endl
		<< "    --guard MACRO      use given guard MACRO in header" << endl
		<< "    -g MACRO           same as --guard" << endl
		<< "    --help             print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setVariableName(const string& name) {
	varname = name;
}

void Options::setExportMacro(const string& macro) {
	expmacro = macro;
}

void Options::setExtraInclude(const string& include) {
	this->include = include;
}

void Options::setGenerateHeader(bool header) {
	this->header = header;
}

void Options::setBlobPath(const string& path) {
	blob = path;
}

void Options::setGuardMacro(const string& macro) {
	guard = macro;
}

void Options::addBareword(const string& word) {
	switch(barewordCount) {
		case 0u:
			infile = word;
			break;
		case 1u:
			outfile = word;
			break;
		default:
			throw UnexpectedBarewordError(word);
	}
	++barewordCount;
}

void Options::checkBarewords() {}
