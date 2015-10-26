#include <iostream>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Decoder16.hpp>
#include <redstrain/text/Encoder16.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/text/CodecManager.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/platform/Filesystem.hpp>
#include <redstrain/io/FormattedOutputStream.hpp>
#include <redstrain/text/Decoder16InputStream.hpp>
#include <redstrain/text/Encoder16OutputStream.hpp>
#include <redstrain/locale/messages.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"

using std::map;
using std::set;
using std::list;
using std::cout;
using std::endl;
using std::string;
using redengine::util::Delete;
using redengine::text::Char16;
using redengine::text::String16;
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::text::Transcode;
using redengine::io::LineOriented;
using redengine::io::StreamCloser;
using redengine::text::StringUtils;
using redengine::text::CodecManager;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::platform::Filesystem;
using redengine::locale::MessageCache;
using redengine::cmdline::OptionLogic;
using redengine::io::FormattedInputStream;
using redengine::io::FormattedOutputStream;
using redengine::text::Decoder16InputStream;
using redengine::text::Encoder16OutputStream;
using redengine::io::MissingInputSeparatorError;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::locale::readMessages;
using redengine::cmdline::runWithOptions;
using redengine::locale::getMessageKeyOrder;

int run(const string&, const Options&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("reference", &Options::setReferenceFile, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('f', &Options::setReferenceFile, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("spec-encoding", &Options::setSpecificationFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('s', &Options::setSpecificationFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("message-encoding", &Options::setMessagesFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('m', &Options::setMessagesFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("reference-encoding", &Options::setReferenceFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addShortOption('r', &Options::setReferenceFileEncoding, OptionLogic::REQUIRED_ARGUMENT);
	logic.addLongOption("verbose", &Options::setVerbose);
	logic.addShortOption('v', &Options::setVerbose);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string&, const Options& options) {
	CodecManager codecs;
	codecs.registerBuiltins();
	codecs.registerBlobs();
	// read specification
	FileInputStream specIn(options.getSpecificationFile());
	StreamCloser closeSpecIn(specIn);
	Delete<Decoder16> decoder(codecs.newDecoder16(options.getSpecificationFileEncoding()));
	Decoder16InputStream decSpecIn(specIn, **decoder);
	MessageCache<Char16> specCache;
	readMessages<Char16>(options.getSpecificationFile(), decSpecIn, specCache, NULL);
	closeSpecIn.close();
	map<String16, size_t> specOrder;
	getMessageKeyOrder<Char16>(specCache, specOrder);
	// read messages
	list<String16> retainedLines;
	set<String16> presentKeys;
	LineOriented::LinebreakPolicy linebreaks;
	bool msgFilePresent = Filesystem::access(options.getMessagesFile(), Filesystem::FILE_EXISTS);
	if(!msgFilePresent)
		linebreaks = LineOriented::AUTO_LINEBREAKS;
	else {
		list<String16> stanza;
		delete decoder.set();
		FileInputStream msgIn(options.getMessagesFile());
		StreamCloser closeMsgIn(msgIn);
		decoder = codecs.newDecoder16(options.getMessagesFileEncoding());
		Decoder16InputStream decMsgIn(msgIn, **decoder);
		FormattedInputStream<Char16> fmtMsgIn(decMsgIn);
		String16 line;
		unsigned lno = 0u;
		while(fmtMsgIn.readLine(line)) {
			++lno;
			stanza.push_back(line);
			line = StringUtils<Char16>::trim(line, StringUtils<Char16>::TRIM_FRONT);
			if(line.empty())
				continue;
			if(line[0] == static_cast<Char16>('#')) {
				line.clear();
				continue;
			}
			String16::size_type pos = line.find(static_cast<Char16>('='));
			if(pos == String16::npos)
				throw MissingInputSeparatorError("=", options.getMessagesFile(), lno);
			String16 key(line.substr(static_cast<String16::size_type>(0u), pos));
			if(specOrder.find(key) == specOrder.end()) {
				if(options.isVerbose())
					cout << "removing " << Transcode::bmpToUTF8(key) << endl;
				stanza.clear();
			}
			else {
				presentKeys.insert(key);
				for(; !stanza.empty(); stanza.pop_front())
					retainedLines.push_back(stanza.front());
			}
			line.clear();
		}
		linebreaks = fmtMsgIn.getLinebreakPolicy();
		closeMsgIn.close();
	}
	// read reference
	MessageCache<Char16> refCache;
	map<String16, size_t> refOrder;
	if(!options.getReferenceFile().empty()) {
		delete decoder.set();
		FileInputStream refIn(options.getReferenceFile());
		StreamCloser closeRefIn(refIn);
		decoder = codecs.newDecoder16(options.getReferenceFileEncoding());
		Decoder16InputStream decRefIn(refIn, **decoder);
		readMessages<Char16>(options.getReferenceFile(), decRefIn, refCache, NULL);
		closeRefIn.close();
		getMessageKeyOrder<Char16>(refCache, refOrder);
	}
	// write messages
	if(!msgFilePresent && options.isVerbose())
		cout << "creating " << options.getMessagesFile() << endl;
	FileOutputStream msgOut(options.getMessagesFile());
	StreamCloser closeMsgOut(msgOut);
	Delete<Encoder16> encoder(codecs.newEncoder16(options.getMessagesFileEncoding()));
	Encoder16OutputStream encMsgOut(msgOut, **encoder);
	FormattedOutputStream<Char16> fmtMsgOut(encMsgOut, linebreaks);
	list<String16>::const_iterator rlbegin(retainedLines.begin()), rlend(retainedLines.end());
	for(; rlbegin != rlend; ++rlbegin)
		fmtMsgOut.println(*rlbegin);
	String16 comment(Transcode::utf8ToBMP("# ")), todo(Transcode::utf8ToBMP("TODO"));
	MessageCache<Char16>::ItemIterator cibegin, ciend;
	specCache.getMessages(cibegin, ciend);
	for(; cibegin != ciend; ++cibegin) {
		if(presentKeys.find(cibegin->getKey()) == presentKeys.end()) {
			if(options.isVerbose())
				cout << "adding " << Transcode::bmpToUTF8(cibegin->getKey()) << endl;
			if(!cibegin->getValue().empty()) {
				fmtMsgOut.print(comment);
				fmtMsgOut.println(cibegin->getValue());
			}
			map<String16, size_t>::const_iterator refit(refOrder.find(cibegin->getKey()));
			String16 refval;
			if(refit != refOrder.end())
				refval = refCache.getMessage(refit->second).getValue();
			if(!refval.empty()) {
				fmtMsgOut.print(comment);
				fmtMsgOut.println(todo);
			}
			fmtMsgOut.print(cibegin->getKey());
			fmtMsgOut.print(static_cast<Char16>('='));
			if(refval.empty())
				fmtMsgOut.println(todo);
			else
				fmtMsgOut.println(refval);
		}
	}
	closeMsgOut.close();
	return 0;
}
