#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>

#include "PathedURI.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::util::MemorySize;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Decoder16;

namespace redengine {
namespace vfs {

	PathedURI::PathedURI() {}

	PathedURI::PathedURI(const String16& path) : path(path) {}

	PathedURI::PathedURI(const PathedURI& uri) : URI(uri), path(uri.path), pathOctets(uri.pathOctets) {}

	PathedURI::~PathedURI() {}

	void PathedURI::setPathedOctetsFromRendition(const String16& fullURI, MemorySize pathOffset,
			void (*byteizeChar)(Char16, string&)) {
		URI::unescape<Char16>(fullURI, pathOffset, path, pathOctets, byteizeChar);
	}

	void PathedURI::pathedDecodeOriginal(const string& octets, string& original) const {
		original = octets;
	}

	void PathedURI::pathedDecodeOriginal(const string& octets, String16& original) const {
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(octets, original, utf8);
	}

	void PathedURI::pathedDecodeOriginal(const string& octets, String32& original) const {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		Transcode::transcodeString3<char, Char16, Char32>(octets, original, utf8, utf16);
	}

	string PathedURI::getRawPath8() const {
		return Transcode::bmpToUTF8(path);
	}

	String16 PathedURI::getRawPath16() const {
		return path;
	}

	String32 PathedURI::getRawPath32() const {
		UTF16Decoder16 utf16;
		String32 result;
		Transcode::transcodeString2<Char16, Char32>(path, result, utf16);
		return result;
	}

	string PathedURI::getPath() const {
		return pathOctets;
	}

#define makeGetOriginal(bits, stype) \
	stype PathedURI::getPath ## bits() const { \
		stype result; \
		pathedDecodeOriginal(getPath(), result); \
		return result; \
	}

	makeGetOriginal(8, string)
	makeGetOriginal(16, String16)
	makeGetOriginal(32, String32)

}}
