#include <redstrain/util/Delete.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/cmdline/parseopt.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/io/FormattedInputStream.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>

#include "Options.hpp"
#include "MissingSeparatorError.hpp"

using std::string;
using redengine::util::Delete;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::platform::Console;
using redengine::util::StringUtils;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::cmdline::runWithOptions;
using redengine::io::FormattedInputStream;
using redengine::io::FormattedOutputStream;
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
	FormattedInputStream<char> fin(**in);
	FormattedOutputStream<char> fout(**out);
	fout.println("#include <redstrain/vfs/BlobVFS.hpp>");
	fout.endLine();
	string line, prefix(options.getPathPrefix());
	if(!prefix.empty()) {
		if(prefix[prefix.length() - static_cast<string::size_type>(1u)] != '/')
			prefix += '/';
	}
	unsigned lno = 0u, id = 0u;
	while(fin.readLine(line)) {
		++lno;
		string::size_type pos = line.find('#');
		if(pos != string::npos)
			line.erase(pos);
		line = StringUtils::trim(line);
		if(line.empty())
			continue;
		pos = line.find('=');
		if(pos == string::npos)
			throw MissingSeparatorError(lno);
		fout.print("static ::redengine::vfs::BlobVFS::BlobLinker alias");
		fout.print(StringUtils::toString(id++));
		fout.print("(\"");
		fout.print(line.substr(pos + static_cast<string::size_type>(1u)));
		fout.print(options.getFileSuffix());
		fout.print("\", \"");
		fout.print(prefix);
		fout.print(line.substr(static_cast<string::size_type>(0u), pos));
		fout.print(options.getFileSuffix());
		fout.println("\");");
		line.clear();
	}
	fout.close();
	fin.close();
	return 0;
}
