#include <iostream>
#include <redstrain/cmdline/StopExecution.hpp>
#include <redstrain/io/CPPArrayOutputStream.hpp>

#include "Options.hpp"

using std::cout;
using std::endl;
using std::string;
using redengine::cmdline::StopExecution;
using redengine::io::CPPArrayOutputStream;

Options::Options(const Options& options) : barewordCount(options.barewordCount), progname(options.progname),
		infile(options.infile), outfile(options.outfile), varname(options.varname) {}

Options::Options(const char* progname) : barewordCount(0u), progname(progname), infile("-"), outfile("-"),
		varname(CPPArrayOutputStream::DEFAULT_VARIABLE_NAME) {}

void Options::usage() {
	cout
		<< "Usage: " << progname << " [options] [input-file [output-file]]" <<  endl
		<< "Options:" << endl
		<< "    --variable NAME    name the generated constant NAME instead" << endl;
	//TODO
	throw StopExecution(0);
}

void Options::setVariableName(const string& name) {
	varname = name;
}

void Options::addBareword(const string& word) {
	//TODO
}

void Options::checkBarewords() {
	//TODO
}
