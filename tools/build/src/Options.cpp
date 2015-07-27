#include <iostream>
#include <redstrain/cmdline/StopExecution.hpp>

#include "Options.hpp"

using std::cout;
using std::endl;
using std::string;
using redengine::cmdline::StopExecution;

Options::Options(const Options& options)
		: progname(options.progname), valves(options.valves), bootstrap(options.bootstrap), base(options.base) {}

Options::Options(const char* progname) : progname(progname), bootstrap(false), base(".") {}

void Options::getValves(ValveNameIterator& begin, ValveNameIterator& end) const {
	begin = valves.begin();
	end = valves.end();
}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [valve...]" << endl
		<< "Options:" << endl
		<< "    --bootstrap    forego use of the Redwood subsystem and thus do not read the" << endl
		<< "                   usual files; emulate Xake instead (useful to build RedStrain" << endl
		<< "                   itsself, as bootstrap mode can be used without blobs)" << endl
		<< "    --base DIR     use DIR as the project base directory (defaults to '.')" << endl
		<< "    --dry          perform dry run: print only what would be done, without" << endl
		<< "                   actually performing the actions" << endl
		<< "    -y             same as --dry" << endl
		<< "    --help         print this helpful message and quit" << endl;
	throw StopExecution(0);
}

void Options::setBootstrap(bool bootstrap) {
	this->bootstrap = bootstrap;
}

void Options::setDry(bool dry) {
	this->dry = dry;
}

void Options::setBase(const string& base) {
	this->base = base;
}

void Options::addBareword(const string& word) {
	valves.push_back(word);
}

void Options::checkBarewords() {}
