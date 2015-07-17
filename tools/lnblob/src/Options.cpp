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
		infile(options.infile), outfile(options.outfile), pathPrefix(options.pathPrefix),
		fileSuffix(options.fileSuffix) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), infile("-"), outfile("-") {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [input-file [output-file]]" <<  endl
		<< "Options:" << endl
		<< "    --prefix PATH      use PATH/ as a prefix to all symlink names" << endl
		<< "    -p PATH            same as --prefix" << endl
		<< "    --suffix EXT       use EXT as a suffix to all symlink names and targets" << endl
		<< "    -s EXT             same as --suffix" << endl
		<< "    --help             print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setPathPrefix(const string& prefix) {
	pathPrefix = prefix;
}

void Options::setFileSuffix(const string& suffix) {
	fileSuffix = suffix;
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
