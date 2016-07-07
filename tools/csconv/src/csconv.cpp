#include <map>
#include <iostream>
#include <redstrain/util/Delete.hpp>
#include <redstrain/text/Transcode.hpp>
#include <redstrain/io/StreamCloser.hpp>
#include <redstrain/platform/Console.hpp>
#include <redstrain/text/CodecManager.hpp>
#include <redstrain/io/FileInputStream.hpp>
#include <redstrain/io/FileOutputStream.hpp>
#include <redstrain/cmdline/parseopt.hpp>

#include "Options.hpp"
#include "blobdepend.hpp"

using std::map;
using std::set;
using std::cout;
using std::endl;
using std::string;
using redengine::util::Delete;
using redengine::text::Char16;
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
	return runWithOptions(argv, argc, logic, &Options::addBareword, &Options::checkBarewords, run,
			APPROPRIATE_CONSOLE_ERROR_HANDLER);
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
	Transcode::transcodeStream3<char, Char16, char>(**in, **out, **decoder, **encoder);
	outCloser.close();
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
	map<string, int>::const_iterator kbegin(known.begin()), kend(known.end());
	string enccname, deccname;
	for(; kbegin != kend; ++kbegin) {
		cout << kbegin->first;
		switch(kbegin->second & CAN_XCODE) {
			case CAN_ENCODE:
				enccname = codecs.getEncoder16CanonicalName(kbegin->first);
				if(enccname == kbegin->first)
					enccname.clear();
				if(enccname.empty())
					cout << " (encode only)";
				else
					cout << " (same as " << enccname << ", encode only)";
				break;
			case CAN_DECODE:
				deccname = codecs.getDecoder16CanonicalName(kbegin->first);
				if(deccname == kbegin->first)
					deccname.clear();
				if(deccname.empty())
					cout << " (decode only)";
				else
					cout << " (same as " << deccname << ", decode only)";
				break;
			case CAN_XCODE:
				enccname = codecs.getEncoder16CanonicalName(kbegin->first);
				if(enccname == kbegin->first)
					enccname.clear();
				deccname = codecs.getDecoder16CanonicalName(kbegin->first);
				if(deccname == kbegin->first)
					deccname.clear();
				if(!enccname.empty() && enccname == deccname)
					cout << " (same as " << enccname << ')';
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
