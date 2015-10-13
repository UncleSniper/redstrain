#include <map>
#include <iostream>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Encoder16.hpp>
#include <redstrain/text/Decoder16.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/text/CodecManager.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/text/Encoder16OutputStream.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"

using std::map;
using std::set;
using std::cout;
using std::endl;
using std::string;
using redengine::util::Delete;
using redengine::text::Encoder16;
using redengine::text::Decoder16;
using redengine::text::Transcode;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::StreamCloser;
using redengine::platform::Console;
using redengine::text::CodecManager;
using redengine::io::FileInputStream;
using redengine::io::FileOutputStream;
using redengine::cmdline::OptionLogic;
using redengine::text::Encoder16OutputStream;
using redengine::cmdline::ConfigurationObjectOptionLogic;
using redengine::cmdline::runWithOptions;

int run(const string&, const Options&);
int listCodecs(CodecManager&);

int main(int argc, char** argv) {
	Options options(*argv);
	ConfigurationObjectOptionLogic<Options> logic(&options);
	logic.addLongOption("help", &Options::usage);
	logic.addLongOption("list-codecs", &Options::setListCodecs);
	logic.addShortOption('l', &Options::setListCodecs);
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run);
}

int run(const string&, const Options& options) {
	CodecManager codecs;
	codecs.registerBuiltins();
	codecs.registerBlobs();
	if(options.isListCodecs())
		return listCodecs(codecs);
	string inencoding(options.getInputEncoding()), outencoding(options.getOutputEncoding());
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
	Delete<Decoder16> decoder(codecs.newDecoder16(inencoding));
	Delete<Encoder16> encoder(codecs.newEncoder16(outencoding));
	Encoder16OutputStream encodingOut(**out, **encoder);
	StreamCloser encodingOutCloser(encodingOut);
	outCloser.release();
	Transcode::decode(**in, encodingOut, **decoder);
	encodingOutCloser.close();
	inCloser.close();
	return 0;
}

int listCodecs(CodecManager& codecs) {
	#define CAN_ENCODE 1
	#define CAN_DECODE 2
	#define CAN_XCODE  (CAN_ENCODE | CAN_DECODE)
	map<string, int> known;
	set<string> names;
	set<string>::const_iterator nbegin, nend;
	codecs.listKnownEncoders16(names);
	for(nbegin = names.begin(), nend = names.end(); nbegin != nend; ++nbegin)
		known[*nbegin] = CAN_ENCODE;
	names.clear();
	codecs.listKnownDecoders16(names);
	for(nbegin = names.begin(), nend = names.end(); nbegin != nend; ++nbegin) {
		map<string, int>::iterator it(known.find(*nbegin));
		if(it == known.end())
			known[*nbegin] = CAN_DECODE;
		else
			it->second |= CAN_DECODE;
	}
	map<CodecManager::Encoder16Factory*, string> sameEncoder;
	map<CodecManager::Encoder16Factory*, string>::const_iterator sencit;
	map<CodecManager::Decoder16Factory*, string> sameDecoder;
	map<CodecManager::Decoder16Factory*, string>::const_iterator sdecit;
	map<string, int>::const_iterator kbegin(known.begin()), kend(known.end());
	bool isSame;
	for(; kbegin != kend; ++kbegin) {
		cout << kbegin->first;
		isSame = false;
		CodecManager::Encoder16Factory* encFac;
		CodecManager::Decoder16Factory* decFac;
		switch(kbegin->second & CAN_XCODE) {
			case CAN_ENCODE:
				encFac = codecs.getEncoder16Factory(kbegin->first);
				if(encFac) {
					sencit = sameEncoder.find(encFac);
					if(sencit != sameEncoder.end()) {
						cout << " (same as " << sencit->second << ", encode only)";
						isSame = true;
					}
					else
						sameEncoder[encFac] = kbegin->first;
				}
				if(!isSame)
					cout << " (encode only)";
				break;
			case CAN_DECODE:
				decFac = codecs.getDecoder16Factory(kbegin->first);
				if(decFac) {
					sdecit = sameDecoder.find(decFac);
					if(sdecit != sameDecoder.end()) {
						cout << " (same as " << sdecit->second << ", decode only)";
						isSame = true;
					}
					else
						sameDecoder[decFac] = kbegin->first;
				}
				if(!isSame)
					cout << " (decode only)";
				break;
			case CAN_XCODE:
				encFac = codecs.getEncoder16Factory(kbegin->first);
				decFac = codecs.getDecoder16Factory(kbegin->first);
				sencit = sameEncoder.find(encFac);
				sdecit = sameDecoder.find(decFac);
				if(sencit != sameEncoder.end() && sdecit != sameDecoder.end() && sencit->second == sdecit->second)
					cout << " (same as " << sencit->second << ')';
				if(encFac && sencit == sameEncoder.end())
					sameEncoder[encFac] = kbegin->first;
				if(decFac && sdecit == sameDecoder.end())
					sameDecoder[decFac] = kbegin->first;
				break;
			default:
				break;
		}
		cout << endl;
	}
	#undef CAN_ENCODE
	#undef CAN_DECODE
	#undef CAN_XCODE
	return 0;
}
