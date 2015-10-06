#include <iostream>
#include <redstrain/cmdline/StopExecution.hpp>

#include "Options.hpp"

using std::cout;
using std::endl;
using std::string;
using redengine::cmdline::StopExecution;

Options::Options(const Options& options)
		: progname(options.progname), goals(options.goals), bootstrap(options.bootstrap), dry(options.dry),
		dumpContext(options.dumpContext), base(options.base) {}

Options::Options(const char* progname)
		: progname(progname), bootstrap(false), dry(false), dumpContext(false), base(".") {}

void Options::getGoals(GoalNameIterator& begin, GoalNameIterator& end) const {
	begin = goals.begin();
	end = goals.end();
}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [goal...]" << endl
		<< "Options:" << endl
		<< "    --bootstrap    forego use of the Redwood subsystem and thus do not read the" << endl
		<< "                   usual files; emulate Xake instead (useful to build RedStrain" << endl
		<< "                   itsself, as bootstrap mode can be used without blobs)" << endl
		<< "    --base DIR     use DIR as the project base directory (defaults to '.')" << endl
		<< "    --dry          perform dry run: print only what would be done, without" << endl
		<< "                   actually performing the actions" << endl
		<< "    -y             same as --dry" << endl
		<< "    --list-goals   list defined goals and quit" << endl
		<< "    -l             same as --list-goals" << endl
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

void Options::setDumpContext(bool dumpContext) {
	this->dumpContext = dumpContext;
}

void Options::setShowGoals(bool showGoals) {
	this->showGoals = showGoals;
}

void Options::addBareword(const string& word) {
	goals.push_back(word);
}

void Options::checkBarewords() {}
