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
		infile(options.infile), outfile(options.outfile), specfile(options.specfile), inenc(options.inenc),
		specenc(options.specenc) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), infile("-"), outfile("-"),
		inenc("UTF-8"), specenc("UTF-8") {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [input-file [output-file]]" << endl
		<< "Options:" << endl
		<< "    --spec FILE                      use specified specification FILE to" << endl
		<< "                                     determine message order" << endl
		<< "    -p FILE                          same as --spec" << endl
		<< "    --message-encoding ENCODING      assume specified ENCODING for <input-file>" << endl
		<< "    -m ENCODING                      same as --message-encoding" << endl
		<< "    --spec-encoding ENCODING         assume specified ENCODING for file" << endl
		<< "                                     specified with --spec" << endl
		<< "    -s ENCODING                      same as --spec-encoding" << endl
		<< "    --help                           print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setSpecificationFile(const string& path) {
	infile = path;
}

void Options::setInputFileEncoding(const string& encoding) {
	inenc = encoding;
}

void Options::setSpecificationFileEncoding(const string& encoding) {
	specenc = encoding;
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
