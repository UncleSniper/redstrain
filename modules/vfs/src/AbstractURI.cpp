#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>

#include "AbstractURI.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::UTF8Decoder16;
using redengine::text::UTF16Decoder16;

namespace redengine {
namespace vfs {

	AbstractURI::AbstractURI(const string& scheme) : scheme(scheme) {}

	AbstractURI::AbstractURI(const AbstractURI& uri) : URI(uri), scheme(uri.scheme) {}

	AbstractURI::~AbstractURI() {}

	string AbstractURI::getScheme8() const {
		return scheme;
	}

	String16 AbstractURI::getScheme16() const {
		return Transcode::utf8ToBMP(scheme);
	}

	String32 AbstractURI::getScheme32() const {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		return Transcode::transcodeShortString3<char, Char16, Char32>(scheme, utf8, utf16);
	}

}}
