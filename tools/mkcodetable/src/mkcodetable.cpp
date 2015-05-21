#include <redstrain/util/Delete.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/text/CachedCodeTable.hpp>
#include <redstrain/text/CodeTableDefinitionReader.hpp>

#include "Options.hpp"

using std::string;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::text::CachedCodeTable;
using redengine::cmdline::runWithOptions;
using redengine::text::CodeTableDefinitionReader;
using redengine::cmdline::ConfigurationObjectOptionLogic;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string&, const Options& options) {
	string infile(options.getInputFile()), outfile(options.getOutputFile());
	Delete<InputStream<char> > in;
	if(infile == "-")
		in = new FileInputStream(Console::getStandardHandle(Console::STANDARD_INPUT));
	else
		in = new FileInputStream(infile);
	CachedCodeTable<Char16> table;
	CodeTableDefinitionReader<Char16, char>::CachedCodeTableSink sink(table);
	CodeTableDefinitionReader<Char16, char>::readDefinition(**in, sink);
	in->close();
	Delete<OutputStream<char> > out;
	if(outfile == "-")
		out = new FileOutputStream(Console::getStandardHandle(Console::STANDARD_OUTPUT));
	else
		out = new FileOutputStream(outfile);
	table.saveTo(**out);
	out->close();
	return 0;
}
