#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Encoder8.hpp>
#include <redstrain/text/UTF16Decoder8.hpp>

#include "URI.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::MemorySize;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Encoder8;
using redengine::text::UTF16Decoder8;
using redengine::text::UTF16Encoder16;
using redengine::text::UTF16Decoder16;
using redengine::text::IllegalCodeError;
using redengine::util::BO_BIG_ENDIAN;
using redengine::util::BO_LITTLE_ENDIAN;

namespace redengine {
namespace vfs {

	const char URI::HEX_DIGITS[16] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	URI::URI() {}

	URI::URI(const URI&) {}

	URI::~URI() {}

	bool URI::isRelativeReference() const {
		return false;
	}

	/* reserved = ";" | "/" | "?" | ":" | "@" | "&" | "=" | +"+" | "$" | ","
	 * unreserved = alphanum | mark
	 * mark = "-" | "_" | "." | "!" | "~" | "*" | "'" | "(" | ")"
	 */

	// rendition -[per scheme]-> octets               -[per protocol]-> original chars
	// €%C2%A2                   \xE2\x82\xAC\xC2\xA2                   €¢

	void URI::byteizeID(const string& source, string& target) {
		target = source;
	}

	void URI::byteizeUTF8(const String16& string16, string& string8) {
		UTF8Encoder16 utf8;
		Transcode::transcodeString2<Char16, char>(string16, string8, utf8);
	}

	void URI::byteizeUTF16BE(const String32& string32, string& string8) {
		UTF16Encoder8 utf16;
		utf16.setByteOrder(BO_BIG_ENDIAN);
		utf16.setFlags(UTF16Encoder8::FL_SUPPRESS_BOM);
		Transcode::transcodeString2<Char32, char>(string32, string8, utf16);
	}

	void URI::byteizeUTF16LE(const String32& string32, string& string8) {
		UTF16Encoder8 utf16;
		utf16.setByteOrder(BO_LITTLE_ENDIAN);
		utf16.setFlags(UTF16Encoder8::FL_SUPPRESS_BOM);
		Transcode::transcodeString2<Char32, char>(string32, string8, utf16);
	}

	void URI::byteizeUTF16UTF8(const String32& string32, string& string8) {
		UTF16Encoder16 utf16;
		UTF8Encoder16 utf8;
		Transcode::transcodeString3<Char32, Char16, char>(string32, string8, utf16, utf8);
	}

	void URI::byteizeCharID(char c, string& octets) {
		octets += c;
	}

	void URI::byteizeCharUTF8(Char16 c, string& octets) {
		char buffer[4];
		MemorySize outsize = UTF8Encoder16::encodeSingleChar(c, buffer);
		octets.append(buffer, static_cast<string::size_type>(outsize));
	}

	void URI::byteizeCharUTF16BE(Char32 c, string& octets) {
		char buffer[4];
		MemorySize outsize = UTF16Encoder8::encodeSingleChar(c, buffer, BO_BIG_ENDIAN);
		octets.append(buffer, static_cast<string::size_type>(outsize));
	}

	void URI::byteizeCharUTF16LE(Char32 c, string& octets) {
		char buffer[4];
		MemorySize outsize = UTF16Encoder8::encodeSingleChar(c, buffer, BO_LITTLE_ENDIAN);
		octets.append(buffer, static_cast<string::size_type>(outsize));
	}

	void URI::byteizeCharUTF16UTF8(Char32 c, string& octets) {
		Char16 buffer[2];
		MemorySize size16 = UTF16Encoder16::encodeSingleChar(c, buffer), u;
		for(u = static_cast<MemorySize>(0u); u < size16; ++u)
			URI::byteizeCharUTF8(buffer[u], octets);
	}

	void URI::renderID(const string& source, string& target) {
		target = source;
	}

	void URI::renderUTF8(const string& string8, String16& string16) {
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(string8, string16, utf8);
	}

	void URI::renderUTF16BE(const string& string8, String32& string32) {
		UTF16Decoder8 utf16;
		utf16.setByteOrder(BO_BIG_ENDIAN);
		Transcode::transcodeString2<char, Char32>(string8, string32, utf16);
	}

	void URI::renderUTF16LE(const string& string8, String32& string32) {
		UTF16Decoder8 utf16;
		utf16.setByteOrder(BO_LITTLE_ENDIAN);
		Transcode::transcodeString2<char, Char32>(string8, string32, utf16);
	}

	void URI::renderUTF8UTF16(const string& string8, String32& string32) {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		Transcode::transcodeString3<char, Char16, Char32>(string8, string32, utf8, utf16);
	}

	MemorySize URI::renderCharID(const char* input, MemorySize, char& output) {
		output = *input;
		return static_cast<MemorySize>(1u);
	}

	MemorySize URI::renderCharUTF8(const char* input, MemorySize insize, Char16& output) {
		return UTF8Decoder16::decodeSingleChar(input, insize, output);
	}

	MemorySize URI::renderCharUTF16BE(const char* input, MemorySize insize, Char32& output) {
		return UTF16Decoder8::decodeSingleChar(input, insize, output, BO_BIG_ENDIAN);
	}

	MemorySize URI::renderCharUTF16LE(const char* input, MemorySize insize, Char32& output) {
		return UTF16Decoder8::decodeSingleChar(input, insize, output, BO_LITTLE_ENDIAN);
	}

	MemorySize URI::renderCharUTF8UTF16(const char* input, MemorySize insize, Char32& output) {
		Char16 c16;
		MemorySize consumed = UTF8Decoder16::decodeSingleChar(input, insize, c16), outcount;
		UTF16Decoder16 utf16;
		while(!utf16.transcodeBlock(&c16, static_cast<MemorySize>(1u),
				&output, static_cast<MemorySize>(1u), outcount)) {}
		if(!outcount)
			utf16.transcodeBlock(NULL, static_cast<MemorySize>(0u), &output, static_cast<MemorySize>(1u), outcount);
		if(outcount)
			return consumed;
		if(consumed >= insize)
			throw IllegalCodeError();
		consumed += UTF8Decoder16::decodeSingleChar(input + consumed, insize - consumed, c16);
		while(!utf16.transcodeBlock(&c16, static_cast<MemorySize>(1u),
				&output, static_cast<MemorySize>(1u), outcount)) {}
		if(!outcount) {
			utf16.transcodeBlock(NULL, static_cast<MemorySize>(0u), &output, static_cast<MemorySize>(1u), outcount);
			if(!outcount)
				throw IllegalCodeError();
		}
		return consumed;
	}

}}
