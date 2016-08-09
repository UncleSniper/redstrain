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
		infile(options.infile), outfile(options.outfile) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), infile("-"), outfile("-") {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [input-file [output-file]]" << endl;
	throw StopExecution(0);
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
