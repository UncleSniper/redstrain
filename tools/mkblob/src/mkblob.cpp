#include <redstrain/util/Delete.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/CPPArrayOutputStream.hpp>

#include "Options.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::cmdline::runWithOptions;
using redengine::io::CPPArrayOutputStream;
using redengine::cmdline::ConfigurationObjectOptionLogic;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("variable", &Options::setVariableName, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('n', &Options::setVariableName, OptionLogic::REQUIRED_ARGUMENT);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string&, const Options& options) {
	string infile(options.getInputFile()), outfile(options.getOutputFile());
	Delete<InputStream<char> > in;
	if(infile == "-")
		in = new FileInputStream(Console::getStandardHandle(Console::STANDARD_INPUT));
	else
		in = new FileInputStream(infile);
	Delete<OutputStream<char> > out;
	if(outfile == "-")
		out = new FileOutputStream(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	else
		out = new FileOutputStream(outfile);
	CPPArrayOutputStream gen(**out, options.getVariableName());
	in->copyTo(gen);
	gen.close();
	in->close();
	return 0;
}
