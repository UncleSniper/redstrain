#include <redstrain/io/MemoryInputStream.hpp>
#include <redstrain/io/StringOutputStream.hpp>

#include "Transcode.hpp"
#include "UTF8Encoder16.hpp"
#include "UTF8Decoder16.hpp"
#include "tweaks.hpp"

using std::string;
using redengine::util::FileSize;
using redengine::io::InputStream;
using redengine::io::OutputStream;
using redengine::util::MemorySize;
using redengine::io::MemoryInputStream;
using redengine::io::StringOutputStream;

namespace redengine {
namespace text {

	static const MemorySize BYTE_BUFFER_SIZE = static_cast<MemorySize>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES);
	static const MemorySize CHAR16_BUFFER_SIZE = static_cast<MemorySize>(REDSTRAIN_TEXT_TRANSCODE_BUFFER_BYTES / 2u);

	FileSize Transcode::encode(InputStream<Char16>& input, OutputStream<char>& output, Encoder16& codec) {
		Char16 inbuf[CHAR16_BUFFER_SIZE];
		char outbuf[BYTE_BUFFER_SIZE];
		MemorySize consumed;
		FileSize total = static_cast<FileSize>(0u);
		for(;;) {
			MemorySize incount = input.read(inbuf, CHAR16_BUFFER_SIZE);
			if(!incount)
				break;
			MemorySize outcount, c;
			for(consumed = static_cast<MemorySize>(0u); consumed < incount; consumed += c) {
				c = codec.encodeBlock(inbuf + consumed, incount - consumed, outbuf, BYTE_BUFFER_SIZE, outcount);
				output.write(outbuf, outcount);
				total += static_cast<FileSize>(outcount);
			}
		}
		codec.endEncoding();
		return total;
	}

	FileSize Transcode::decode(InputStream<char>& input, OutputStream<Char16>& output, Decoder16& codec) {
		char inbuf[BYTE_BUFFER_SIZE];
		Char16 outbuf[CHAR16_BUFFER_SIZE];
		MemorySize consumed;
		FileSize total = static_cast<FileSize>(0u);
		for(;;) {
			MemorySize incount = input.read(inbuf, BYTE_BUFFER_SIZE);
			if(!incount)
				break;
			MemorySize outcount, c;
			for(consumed = static_cast<MemorySize>(0u); consumed < incount; consumed += c) {
				c = codec.decodeBlock(inbuf + consumed, incount - consumed, outbuf, CHAR16_BUFFER_SIZE, outcount);
				output.write(outbuf, outcount);
				total += static_cast<FileSize>(outcount);
			}
		}
		codec.endDecoding();
		return total;
	}

	void Transcode::encode(const String16& input, string& output, Encoder16& codec) {
		MemoryInputStream<Char16> source(input.data(), static_cast<MemorySize>(input.length()));
		StringOutputStream<char> sink(output);
		Transcode::encode(source, sink, codec);
	}

	void Transcode::decode(const string& input, String16& output, Decoder16& codec) {
		MemoryInputStream<char> source(input.data(), static_cast<MemorySize>(input.length()));
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
