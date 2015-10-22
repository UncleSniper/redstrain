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
		specfile(options.specfile), msgfile(options.msgfile) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname) {}

void Options::usage() {
	cout
		<< "Usage: " << progname << "[options] <specification-file> <messages-file>" << endl
		<< "Options:" << endl
		<< "    --spec-encoding ENCODING         " << endl
		<< "    --message-encoding ENCODING      " << endl
		<< "    --help                           print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setSpecificationFileEncoding(const string& encoding) {
	specenc = encoding;
}

void Options::setMessagesFileEncoding(const string& encoding) {
	msgenc = encoding;
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
