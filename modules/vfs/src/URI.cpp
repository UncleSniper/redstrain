#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Encoder.hpp>
#include <redstrain/text/UTF16Encoder8.hpp>

#include "URI.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::UTF8Encoder16;
using redengine::text::UTF16Encoder8;
using redengine::text::UTF16Encoder16;
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

}}
