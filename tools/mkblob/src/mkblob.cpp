#include <redstrain/util/Delete.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/CPPArrayOutputStream.hpp>

#include "Options.hpp"
#include "blobdepend.hpp"

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
	logic.addLongOption("export", &Options::setExportMacro, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('e', &Options::setExportMacro, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("include", &Options::setExtraInclude, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('i', &Options::setExtraInclude, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("header", &Options::setGenerateHeader);
	logic.addShortOption('h', &Options::setGenerateHeader);
	logic.addLongOption("blob", &Options::setBlobPath, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('b', &Options::setBlobPath, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("guard", &Options::setGuardMacro, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('g', &Options::setGuardMacro, OptionLogic::REQUIRED_ARGUMENT);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run,
			APPROPRIATE_CONSOLE_ERROR_HANDLER);
}

int run(const string&, const Options& options) {
	string infile(options.getInputFile()), outfile(options.getOutputFile());
	Delete<InputStream<char> > in;
	if(!options.shouldGenerateHeader()) {
		if(infile == "-")
			in = new FileInputStream(Console::getStandardHandle(Console::STANDARD_INPUT));
		else
			in = new FileInputStream(infile);
	}
	Delete<OutputStream<char> > out;
	if(outfile == "-")
		out = new FileOutputStream(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	else
		out = new FileOutputStream(outfile);
	CPPArrayOutputStream gen(**out, options.getVariableName());
	gen.setExportMacro(options.getExportMacro());
	gen.setExtraInclude(options.getExtraInclude());
	gen.setBlobPath(options.getBlobPath());
	gen.setGuardMacro(options.getGuardMacro());
	if(options.shouldGenerateHeader())
		gen.writeHeader();
	else
		in->copyTo(gen);
	gen.close();
	if(*in)
		in->close();
	return 0;
}
