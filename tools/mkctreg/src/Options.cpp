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
		infile(options.infile), outfile(options.outfile), ns(options.ns) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), infile("-"), outfile("-") {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [input-file [output-file]]" <<  endl
		<< "Options:" << endl
		<< "    --namespace NS      assume codetable blobs are defined in namespace NS" << endl
		<< "    -n NS               same as --namespace" << endl
		<< "    --help              print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setNamespace(const string& ns) {
	this->ns = ns;
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
