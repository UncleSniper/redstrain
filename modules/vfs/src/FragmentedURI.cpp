#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>

#include "FragmentedURI.hpp"

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

	FragmentedURI::FragmentedURI() {}

	FragmentedURI::FragmentedURI(const String16& fragment) : fragment(fragment) {}

	FragmentedURI::FragmentedURI(const FragmentedURI& uri)
			: URI(uri), fragment(uri.fragment), fragmentOctets(uri.fragmentOctets) {}

	FragmentedURI::~FragmentedURI() {}

	void FragmentedURI::setFragmentedOctetsFromRendition(const String16& fullURI, MemorySize fragmentOffset,
			void (*byteizeChar)(Char16, string&)) {
		URI::unescape<Char16>(fullURI, fragmentOffset, fragment, fragmentOctets, byteizeChar);
	}

	void FragmentedURI::fragmentedDecodeOriginal(const string& octets, string& original) const {
		original = octets;
	}

	void FragmentedURI::fragmentedDecodeOriginal(const string& octets, String16& original) const {
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(octets, original, utf8);
	}

	void FragmentedURI::fragmentedDecodeOriginal(const string& octets, String32& original) const {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		Transcode::transcodeString3<char, Char16, Char32>(octets, original, utf8, utf16);
	}

	string FragmentedURI::getRawFragment8() const {
		return Transcode::bmpToUTF8(fragment);
	}

	String16 FragmentedURI::getRawFragment16() const {
		return fragment;
	}

	String32 FragmentedURI::getRawFragment32() const {
		UTF16Decoder16 utf16;
		String32 result;
		Transcode::transcodeString2<Char16, Char32>(fragment, result, utf16);
		return result;
	}

	string FragmentedURI::getFragment() const {
		return fragmentOctets;
	}

#define makeGetOriginal(bits, stype) \
	stype FragmentedURI::getFragment ## bits() const { \
		stype result; \
		fragmentedDecodeOriginal(getFragment(), result); \
		return result; \
	}

	makeGetOriginal(8, string)
	makeGetOriginal(16, String16)
	makeGetOriginal(32, String32)

}}
