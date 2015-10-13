#include <iostream>
#include <redstrain/cmdline/StopExecution.hpp>
#include <redstrain/cmdline/MissingBarewordError.hpp>
#include <redstrain/cmdline/UnexpectedBarewordError.hpp>

#include "Options.hpp"

using std::cout;
using std::endl;
using std::string;
using redengine::cmdline::StopExecution;
using redengine::cmdline::MissingBarewordError;
using redengine::cmdline::UnexpectedBarewordError;

Options::Options(const Options& options) : barewordCount(options.barewordCount), progname(options.progname),
		inencoding(options.inencoding), outencoding(options.outencoding), infile(options.infile),
		outfile(options.outfile), listCodecs(options.listCodecs) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), outencoding("UTF-8"),
		infile("-"), outfile("-"), listCodecs(false) {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] <input-encoding> [output-encoding [input-file [output-file]]]" << endl
		<< "     Note that <output-encoding> defaults to 'UTF-8'." << endl
		<< "Options:" << endl
		<< "    --list-codecs      list available codecs and quit" << endl
		<< "    -l                 same as --list-codecs" << endl
		<< "    --help             print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::addBareword(const string& word) {
	switch(barewordCount) {
		case 0u:
			inencoding = word;
			break;
		case 1u:
			outencoding = word;
			break;
		case 2u:
			infile = word;
			break;
		case 3u:
			outfile = word;
			break;
		default:
			throw UnexpectedBarewordError(word);
	}
	++barewordCount;
}

void Options::checkBarewords() {
	if(!barewordCount && !listCodecs)
		throw MissingBarewordError("input-encoding");
}
