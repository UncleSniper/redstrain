#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/vfs/BlobLinkerDefinitionGenerator.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"
#include "blobdepend.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::platform::Console;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::cmdline::runWithOptions;
using redengine::vfs::BlobLinkerDefinitionGenerator;
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
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run,
			APPROPRIATE_CONSOLE_ERROR_HANDLER);
}

int run(const string&, const Options& options) {
	string infile(options.getInputFile()), outfile(options.getOutputFile());
	Delete<InputStream<char> > in;
	StreamCloser inCloser;
	if(infile == "-")
		in = new FileInputStream(Console::getStandardHandle(Console::STANDARD_INPUT));
	else {
		in = new FileInputStream(infile);
		inCloser = *in;
	}
	Delete<OutputStream<char> > out;
	StreamCloser outCloser;
	if(outfile == "-")
		out = new FileOutputStream(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	else {
		out = new FileOutputStream(outfile);
		outCloser = *out;
	}
	BlobLinkerDefinitionGenerator::FileIncludeResolver
			includeResolver(Pathname::dirname(options.getInputFile(), Pathname::LOGICAL));
	BlobLinkerDefinitionGenerator generator(**in, options.getInputFile(), **out, includeResolver);
	generator.setPathPrefix(options.getPathPrefix());
	generator.setFileSuffix(options.getFileSuffix());
	generator.defineBlobLinkers();
	outCloser.close();
	inCloser.close();
	return 0;
}
