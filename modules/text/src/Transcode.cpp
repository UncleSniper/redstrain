#include <redstrain/io/MemoryInputStream.hpp>
#include <redstrain/io/StringOutputStream.hpp>

#include "Transcode.hpp"
#include "UTF8Encoder16.hpp"
#include "UTF8Decoder16.hpp"
#include "tweaks.hpp"

using std::string;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::io::MemoryInputStream;
using redengine::io::StringOutputStream;

namespace redengine {
namespace text {

	static const size_t BYTE_BUFFER_SIZE = static_cast<size_t>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES);
	static const size_t CHAR16_BUFFER_SIZE = static_cast<size_t>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES / 2u);

	size_t Transcode::encode(InputStream<Char16>& input, OutputStream<char>& output, Encoder16& codec) {
		Char16 inbuf[CHAR16_BUFFER_SIZE];
		char outbuf[BYTE_BUFFER_SIZE];
		size_t consumed, total = static_cast<size_t>(0u);
		for(;;) {
			size_t incount = input.read(inbuf, CHAR16_BUFFER_SIZE);
			if(!incount)
				break;
			size_t outcount, c;
			for(consumed = static_cast<size_t>(0u); consumed < incount; consumed += c) {
				c = codec.encodeBlock(inbuf + consumed, incount - consumed, outbuf, BYTE_BUFFER_SIZE, outcount);
				output.write(outbuf, outcount);
				total += outcount;
			}
		}
		codec.endEncoding();
		return total;
	}

	size_t Transcode::decode(InputStream<char>& input, OutputStream<Char16>& output, Decoder16& codec) {
		char inbuf[BYTE_BUFFER_SIZE];
		Char16 outbuf[CHAR16_BUFFER_SIZE];
		size_t consumed, total = static_cast<size_t>(0u);
		for(;;) {
			size_t incount = input.read(inbuf, BYTE_BUFFER_SIZE);
			if(!incount)
				break;
			size_t outcount, c;
			for(consumed = static_cast<size_t>(0u); consumed < incount; consumed += c) {
				c = codec.decodeBlock(inbuf + consumed, incount - consumed, outbuf, CHAR16_BUFFER_SIZE, outcount);
				output.write(outbuf, outcount);
				total += outcount;
			}
		}
		codec.endDecoding();
		return total;
	}

	void Transcode::encode(const String16& input, string& output, Encoder16& codec) {
		MemoryInputStream<Char16> source(input.data(), static_cast<size_t>(input.length()));
		StringOutputStream<char> sink(output);
		Transcode::encode(source, sink, codec);
	}

	void Transcode::decode(const string& input, String16& output, Decoder16& codec) {
		MemoryInputStream<char> source(input.data(), static_cast<size_t>(input.length()));
		StringOutputStream<Char16> sink(output);
		Transcode::decode(source, sink, codec);
	}

	String16 Transcode::utf8ToBMP(const string& str) {
		UTF8Decoder16 codec;
		String16 result;
		Transcode::decode(str, result, codec);
		return result;
	}

	string Transcode::bmpToUTF8(const String16& str) {
		UTF8Encoder16 codec;
		string result;
		Transcode::encode(str, result, codec);
		return result;
	}

	string _renderNumber16ForError(const String16& number) {
		return Transcode::bmpToUTF8(number);
	}

}}
