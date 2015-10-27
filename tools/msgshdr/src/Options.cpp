#include <iostream>
#include <redstrain/cmdline/StopExecution.hpp>
#include <redstrain/cmdline/UnexpectedBarewordError.hpp>

#include "Options.hpp"

using std::cout;
using std::endl;
using std::string;
using redengine::cmdline::StopExecution;
using redengine::cmdline::UnexpectedBarewordError;

Options::Options(const Options& options) : barewordCount(options.barewordCount), progname(options.progname),
		infile(options.infile), outfile(options.outfile), inenc(options.inenc), type(options.type),
		constprefix(options.constprefix), guard(options.guard) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), infile("-"), outfile("-"),
		inenc("UTF-8"), type("Messages"), constprefix("MSG_") {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [input-file [output-file]]" << endl
		<< "Options:" << endl
		<< "    --spec-encoding ENCODING      assume specified ENCODING for <input-file>" << endl
		<< "                                  (defaults to 'UTF-8')" << endl
		<< "    -s ENCODING                   same as --spec-encoding" << endl
		<< "    --type QUALIFIED_NAME         name the generated type QUALIFIED_NAME" << endl
		<< "                                  (defaults to 'Messages')" << endl
		<< "    -t QUALIFIED_NAME             same as --type" << endl
		<< "    --constant-prefix PREFIX      prefix names of enum constants with PREFIX" << endl
		<< "                                  (defaults to 'MSG_')" << endl
		<< "    -p PREFIX                     same as --constant-prefix" << endl
		<< "    --guard-macro MACRO           use MACRO as the header guard macro" << endl
		<< "    -g MACRO                      same as --guard-macro" << endl
		<< "    --help                        print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setInputFileEncoding(const string& encoding) {
	inenc = encoding;
}

void Options::setTypeName(const string& typeName) {
	type = typeName;
}

void Options::setConstantPrefix(const string& prefix) {
	constprefix = prefix;
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
