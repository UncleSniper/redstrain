#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF8Decoder.hpp>
#include <redstrain/text/UTF16Decoder16.hpp>

#include "QueriedURI.hpp"

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

	QueriedURI::QueriedURI() {}

	QueriedURI::QueriedURI(const String16& query) : query(query) {}

	QueriedURI::QueriedURI(const QueriedURI& uri) : URI(uri), query(uri.query), queryOctets(uri.queryOctets) {}

	QueriedURI::~QueriedURI() {}

	void QueriedURI::setQueriedOctetsFromRendition(const String16& fullURI, MemorySize queryOffset,
			void (*byteizeChar)(Char16, string&)) {
		URI::unescape<Char16>(fullURI, queryOffset, query, queryOctets, byteizeChar);
	}

	void QueriedURI::queriedDecodeOriginal(const string& octets, string& original) const {
		original = octets;
	}

	void QueriedURI::queriedDecodeOriginal(const string& octets, String16& original) const {
		UTF8Decoder16 utf8;
		Transcode::transcodeString2<char, Char16>(octets, original, utf8);
	}

	void QueriedURI::queriedDecodeOriginal(const string& octets, String32& original) const {
		UTF8Decoder16 utf8;
		UTF16Decoder16 utf16;
		Transcode::transcodeString3<char, Char16, Char32>(octets, original, utf8, utf16);
	}

	string QueriedURI::getRawQuery8() const {
		return Transcode::bmpToUTF8(query);
	}

	String16 QueriedURI::getRawQuery16() const {
		return query;
	}

	String32 QueriedURI::getRawQuery32() const {
		UTF16Decoder16 utf16;
		String32 result;
		Transcode::transcodeString2<Char16, Char32>(query, result, utf16);
		return result;
	}

	string QueriedURI::getQuery() const {
		return queryOctets;
	}

#define makeGetOriginal(bits, stype) \
	stype QueriedURI::getQuery ## bits() const { \
		stype result; \
		queriedDecodeOriginal(getQuery(), result); \
		return result; \
	}

	makeGetOriginal(8, string)
	makeGetOriginal(16, String16)
	makeGetOriginal(32, String32)

}}
