#include <redstrain/io/MemoryInputStream.hpp>
#include <redstrain/io/StringOutputStream.hpp>

#include "Transcode.hpp"
#include "UTF8Encoder.hpp"
#include "UTF16Encoder.hpp"
#include "UTF16Decoder.hpp"
#include "UTF8Decoder16.hpp"
#include "TextCodecInputStream.hpp"
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

	String16 Transcode::utf8ToBMP(const string& str) {
		UTF8Decoder16 codec;
		String16 result;
		Transcode::transcodeString2<char, Char16>(str, result, codec);
		return result;
	}

	string Transcode::bmpToUTF8(const String16& str) {
		UTF8Encoder16 codec;
		string result;
		Transcode::transcodeString2<Char16, char>(str, result, codec);
		return result;
	}

	String32 Transcode::utf8ToUnicode(const string& str) {
		UTF8Decoder16 codec816;
		UTF16Decoder codec1632;
		String32 result;
		Transcode::transcodeString3<char, Char16, Char32>(str, result, codec816, codec1632);
		return result;
	}

	string Transcode::unicodeToUTF8(const String32& str) {
		UTF16Encoder codec3216;
		UTF8Encoder16 codec168;
		string result;
		Transcode::transcodeString3<Char32, Char16, char>(str, result, codec3216, codec168);
		return result;
	}

	string _renderNumber16ForError(const String16& number) {
		return Transcode::bmpToUTF8(number);
	}

	string _renderNumber32ForError(const String32& number) {
		return Transcode::unicodeToUTF8(number);
	}

}}
