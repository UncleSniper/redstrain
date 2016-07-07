#include <redstrain/util/Delete.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/Pathname.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/text/BlobCodeTable16Registrar.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"
#include "blobdepend.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::platform::Console;
using redengine::util::StringUtils;
using redengine::platform::Pathname;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::text::BlobCodeTable16Registrar;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::cmdline::runWithOptions;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("namespace", &Options::setNamespace, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('n', &Options::setNamespace, OptionLogic::REQUIRED_ARGUMENT);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run,
			APPROPRIATE_CONSOLE_ERROR_HANDLER);
}

int run(const string&, const Options& options) {
	string infile(options.getInputFile()), outfile(options.getOutputFile());
	string nsPrefix(StringUtils::trim(options.getNamespace()));
	if(!nsPrefix.empty() && !StringUtils::endsWith(nsPrefix, "::"))
		nsPrefix += "::";
	// read
	Delete<InputStream<char> > in;
	StreamCloser inCloser;
	if(infile == "-")
		in = new FileInputStream(Console::getStandardHandle(Console::STANDARD_INPUT));
	else {
		in = new FileInputStream(infile);
		inCloser = *in;
	}
	BlobCodeTable16Registrar::NameCache nameCache;
	BlobCodeTable16Registrar::GeneratorReader::FileIncludeResolver
			includeResolver(Pathname::dirname(infile, Pathname::LOGICAL));
	BlobCodeTable16Registrar::GeneratorReader reader(**in, infile, nameCache, includeResolver);
	reader.readBlobNames();
	inCloser.close();
	// write
	Delete<OutputStream<char> > out;
	StreamCloser outCloser;
	if(outfile == "-")
		out = new FileOutputStream(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	else {
		out = new FileOutputStream(outfile);
		outCloser = *out;
	}
	BlobCodeTable16Registrar::GeneratorWriter writer(**out, NULL);
	BlobCodeTable16Registrar::GeneratorWriter::DefaultBlobSymbolMapper blobSymbolMapper(nsPrefix);
	writer.setBlobSymbolMapper(&blobSymbolMapper);
	nameCache.generateBlobRegistrars(writer);
	outCloser.close();
	return 0;
}
