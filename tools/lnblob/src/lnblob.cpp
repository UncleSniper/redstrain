#include <redstrain/util/Delete.hpp>
#include <redstrain/vfs/BlobVFS.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>

#include "Options.hpp"

using std::string;
using redengine::util::Delete;
using redengine::vfs::BlobVFS;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::cmdline::runWithOptions;
using redengine::cmdline::ConfigurationObjectOptionLogic;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("prefix", &Options::setPathPrefix, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('p', &Options::setPathPrefix, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("suffix", &Options::setFileSuffix, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('s', &Options::setFileSuffix, OptionLogic::REQUIRED_ARGUMENT);
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
	BlobVFS::BlobLinker::generateLinkers(**in, **out, options.getPathPrefix(), options.getFileSuffix());
	out->close();
	in->close();
	return 0;
}
