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
		specfile(options.specfile), msgfile(options.msgfile), reffile(options.reffile), specenc(options.specenc),
		msgenc(options.msgenc), refenc(options.refenc), verbose(options.verbose) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), specenc("UTF-8"), msgenc("UTF-8"),
		refenc("UTF-8"), verbose(false) {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] <specification-file> <messages-file>" << endl
		<< "Options:" << endl
		<< "    --reference FILE                   include messages from specified FILE for" << endl
		<< "                                       reference" << endl
		<< "    -f FILE                            same as --reference" << endl
		<< "    --spec-encoding ENCODING           assume specified ENCODING for" << endl
		<< "                                       <specification-file>" << endl
		<< "    -s ENCODING                        same as --spec-encoding" << endl
		<< "    --message-encoding ENCODING        assume specified ENCODING for" << endl
		<< "                                       <messages-file>" << endl
		<< "    -m ENCODING                        same as --message-encoding" << endl
		<< "    --reference-encoding ENCODING      assume specified ENCODING for file" << endl
		<< "                                       specified with --reference" << endl
		<< "    -r ENCODING                        same as --reference-encoding" << endl
		<< "    --verbose                          print what is done" << endl
		<< "    -v                                 same as --verbose" << endl
		<< "    --help                             print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setReferenceFile(const string& path) {
	reffile = path;
}

void Options::setSpecificationFileEncoding(const string& encoding) {
	specenc = encoding;
}

void Options::setMessagesFileEncoding(const string& encoding) {
	msgenc = encoding;
}

void Options::setReferenceFileEncoding(const string& encoding) {
	refenc = encoding;
}

void Options::setVerbose(bool verbose) {
	this->verbose = verbose;
}

void Options::addBareword(const string& word) {
	switch(barewordCount) {
		case 0u:
			specfile = word;
			break;
		case 1u:
			msgfile = word;
			break;
		default:
			throw UnexpectedBarewordError(word);
	}
	++barewordCount;
}

void Options::checkBarewords() {
	switch(barewordCount) {
		case 0u:
			throw MissingBarewordError("specification-file");
		case 1u:
			throw MissingBarewordError("messages-file");
		default:
			break;
	}
}
