#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>

#include "URIEscapeError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::MemorySize;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Encoder16;
using redengine::text::UTF16Decoder16;

namespace redengine {
namespace vfs {

	static MemorySize conv8(const string& uri, MemorySize originalOffset) {
		UTF8Decoder16 utf8;
		String16 s16;
		Transcode::transcodeString2<char, Char16>(uri.substr(static_cast<string::size_type>(0u),
				static_cast<string::size_type>(originalOffset)), s16, utf8);
		return static_cast<MemorySize>(s16.length());
	}

	static MemorySize ahead8(const string& uri, MemorySize originalOffset) {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		String32 s32;
		Transcode::transcodeString3<char, Char16, Char32>(uri.substr(static_cast<string::size_type>(0u),
				static_cast<string::size_type>(originalOffset)), s32, utf8, utf16);
		return static_cast<MemorySize>(s32.length());
	}

	URIEscapeError::URIEscapeError(const string& uri, MemorySize escapeOffset)
			: InvalidURIError(uri), escapeOffset(conv8(uri, escapeOffset)),
			escapeOffset32(ahead8(uri, escapeOffset)) {}

	static MemorySize ahead16(const String16& uri, MemorySize originalOffset) {
		UTF16Decoder16 utf16;
		String32 s32;
		Transcode::transcodeString2<Char16, Char32>(uri.substr(static_cast<String16::size_type>(0u),
				static_cast<String16::size_type>(originalOffset)), s32, utf16);
		return static_cast<MemorySize>(s32.length());
	}

	URIEscapeError::URIEscapeError(const String16& uri, MemorySize escapeOffset)
			: InvalidURIError(uri), escapeOffset(escapeOffset), escapeOffset32(ahead16(uri, escapeOffset)) {}

	static MemorySize conv32(const String32& uri, MemorySize originalOffset) {
		UTF16Encoder16 utf16;
		String16 s16;
		Transcode::transcodeString2<Char32, Char16>(uri.substr(static_cast<String32::size_type>(0u),
				static_cast<String32::size_type>(originalOffset)), s16, utf16);
		return static_cast<MemorySize>(s16.length());
	}

	URIEscapeError::URIEscapeError(const String32& uri, MemorySize escapeOffset)
			: InvalidURIError(uri), escapeOffset(conv32(uri, escapeOffset)), escapeOffset32(escapeOffset) {}

	URIEscapeError::URIEscapeError(const URIEscapeError& error)
			: Error(error), InvalidURIError(error), escapeOffset(error.escapeOffset),
			escapeOffset32(error.escapeOffset32) {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(URIEscapeError)

}}
