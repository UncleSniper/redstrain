#include <iostream>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Decoder16.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/text/CodecManager.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/text/Decoder16InputStream.hpp>
#include <redstrain/locale/messages.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"

using std::list;
using std::string;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::text::Decoder16;
using redengine::io::StreamCloser;
using redengine::text::CodecManager;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::locale::MessageCache;
using redengine::cmdline::OptionLogic;
using redengine::text::Decoder16InputStream;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::locale::readMessages;
using redengine::cmdline::runWithOptions;
using redengine::locale::getMessageKeyOrder;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("spec", &Options::setSpecificationFile, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('p', &Options::setSpecificationFile, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("message-encoding", &Options::setInputFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('m', &Options::setInputFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("spec-encoding", &Options::setSpecificationFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('s', &Options::setSpecificationFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string&, const Options& options) {
	CodecManager codecs;
	codecs.registerBuiltins();
	codecs.registerBlobs();
	// read specification
	Delete<list<String16> > order;
	Delete<Decoder16> decoder;
	if(!options.getSpecificationFile().empty()) {
		FileInputStream specIn(options.getSpecificationFile());
		StreamCloser closeSpecIn(specIn);
		decoder = codecs.newDecoder16(options.getSpecificationFileEncoding());
		Decoder16InputStream decSpecIn(specIn, **decoder);
		MessageCache<Char16> specCache;
		readMessages<Char16>(options.getSpecificationFile(), decSpecIn, specCache, NULL);
		closeSpecIn.close();
		delete decoder.set();
		order = new list<String16>;
		getMessageKeyOrder<Char16>(specCache, NULL, *order);
	}
	// read messages
	FileInputStream in(options.getInputFile()); //TODO: handle "-"
	StreamCloser closeIn(in);
	decoder = codecs.newDecoder16(options.getInputFileEncoding());
	Decoder16InputStream decIn(in, **decoder);
	MessageCache<Char16> cache;
	readMessages<Char16>(options.getInputFile(), decIn, cache, *order);
	closeIn.close();
	// write messages
	FileOutputStream out(options.getOutputFile()); //TODO: handle "-"
	StreamCloser closeOut(out);
	cache.saveTo(out);
	closeOut.close();
	return 0;
}
