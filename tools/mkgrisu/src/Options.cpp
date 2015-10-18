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
		outfile(options.outfile), ns(options.ns), className(options.className),
		significantsName(options.significantsName), exponentsName(options.exponentsName) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), outfile("-") {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [output-file]" << endl
		<< "Options:" << endl
		<< "    --namespace NAMESPACE    generate source into specified NAMESPACE" << endl
		<< "    -n NAMESPACE             same as --namespace" << endl
		<< "    --class CLASS            generate variables as members of specified CLASS" << endl
		<< "    -c CLASS                 same as --class" << endl
		<< "    --significants NAME      generate significants variable with specified NAME" << endl
		<< "    -s NAME                  same as --significants" << endl
		<< "    --exponents NAME         generate exponents variable with specified NAME" << endl
		<< "    -e NAME                  same as --exponents" << endl
		<< "    --help                   print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setNamespace(const string& ns) {
	this->ns = ns;
}

void Options::setClassName(const string& name) {
	className = name;
}

void Options::setSignificantsName(const string& name) {
	significantsName = name;
}

void Options::setExponentsName(const string& name) {
	exponentsName = name;
}

void Options::addBareword(const string& word) {
	switch(barewordCount) {
		case 0u:
			outfile = word;
			break;
		default:
			throw new UnexpectedBarewordError(word);
	}
	++barewordCount;
}

void Options::checkBarewords() {}
