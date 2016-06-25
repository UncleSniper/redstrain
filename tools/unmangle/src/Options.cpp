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

Options::Options(const Options& options)
		: progname(options.progname), barewordCount(options.barewordCount), scheme(options.scheme),
		symbol(options.symbol), nowrap(options.nowrap) {}

Options::Options(const char* progname) : progname(progname), barewordCount(0u), nowrap(false) {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] <scheme> [symbol]" << endl
		<< "Options:" << endl
		<< "    --nowrap    don't split symbols into multiple lines, even when printing to" << endl
		<< "                a terminal" << endl
		<< "    -w          same as --nowrap" << endl
		<< "    --help      print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::addBareword(const string& word) {
	switch(barewordCount) {
		case 0u:
			scheme = word;
			break;
		case 1u:
			symbol = word;
			break;
		default:
			throw UnexpectedBarewordError(word);
	}
	++barewordCount;
}

void Options::setAvoidWrapping(bool nowrap) {
	this->nowrap = nowrap;
}

void Options::checkBarewords() {
	if(!barewordCount)
		throw MissingBarewordError("scheme");
}
