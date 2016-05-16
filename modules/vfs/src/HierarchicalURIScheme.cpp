#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>

#include "HierarchicalURIScheme.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Encoder16;

namespace redengine {
namespace vfs {

	HierarchicalURIScheme::HierarchicalURIScheme() {}

	HierarchicalURIScheme::HierarchicalURIScheme(const URIScheme& scheme) : URIScheme(scheme) {}

	URI* HierarchicalURIScheme::newURI(const string& specifier, string::size_type sspIndex) {
		String16 head, tail;
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(specifier.substr(static_cast<string::size_type>(0u),
				sspIndex), head, utf8);
		utf8.reset();
		Transcode::transcodeString2<char, Char16>(specifier.substr(sspIndex), tail, utf8);
		return newURI(head + tail, head.length());
	}

	URI* HierarchicalURIScheme::newURI(const String16& specifier, String16::size_type sspIndex) {
		HierarchicalURI::Decomposition decomposition;
		HierarchicalURIScheme::parseHierarchicalURI<Char16>(specifier, sspIndex, decomposition);
		return newURI(specifier, decomposition);
	}

	URI* HierarchicalURIScheme::newURI(const String32& specifier, String32::size_type sspIndex) {
		String16 head, tail;
		UTF16Encoder16 utf16;
		Transcode::transcodeString2<Char32, Char16>(specifier.substr(static_cast<String32::size_type>(0u),
				sspIndex), head, utf16);
		utf16.reset();
		Transcode::transcodeString2<Char32, Char16>(specifier.substr(sspIndex), tail, utf16);
		return newURI(head + tail, head.length());
	}

}}
