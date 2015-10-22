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

using std::map;
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
	logic.addLongOption("spec-encoding", &Options::setSpecificationFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('s', &Options::setSpecificationFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("message-encoding", &Options::setMessagesFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('m', &Options::setMessagesFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string&, const Options& options) {
	CodecManager codecs;
	codecs.registerBuiltins();
	codecs.registerBlobs();
	FileInputStream specIn(options.getSpecificationFile());
	StreamCloser closeSpecIn(specIn);
	Delete<Decoder16> decoder(codecs.newDecoder16(options.getSpecificationFileEncoding()));
	Decoder16InputStream decSpecIn(specIn, **decoder);
	MessageCache<Char16> specCache;
	readMessages<Char16>(options.getSpecificationFile(), decSpecIn, specCache, NULL);
	map<String16, size_t> specOrder;
	getMessageKeyOrder<Char16>(specCache, specOrder);
	//TODO
}
