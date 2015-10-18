#include <redstrain/text/Grisu.hpp>
#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"

using std::string;
using redengine::text::Grisu;
using redengine::util::Delete;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::platform::Console;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::cmdline::runWithOptions;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("namespace", &Options::setNamespace, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('n', &Options::setNamespace, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("class", &Options::setClassName, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('c', &Options::setClassName, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("significants", &Options::setSignificantsName, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('s', &Options::setSignificantsName, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("exponents", &Options::setExponentsName, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('e', &Options::setExponentsName, OptionLogic::REQUIRED_ARGUMENT);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string&, const Options& options) {
	string outfile(options.getOutputFile());
	Delete<OutputStream<char> > out;
	StreamCloser outCloser;
	if(outfile == "-")
		out = new FileOutputStream(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	else {
		out = new FileOutputStream(outfile);
		outCloser = *out;
	}
	Grisu::generateCacheSource(**out, options.getNamespace(), options.getClassName(),
			options.getSignificantsName(), options.getExponentsName());
	outCloser.close();
	return 0;
}
