#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>

#include "InvalidURIError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::UTF16Encoder16;

namespace redengine {
namespace vfs {

	InvalidURIError::InvalidURIError(const string& uri) : uri(Transcode::utf8ToBMP(uri)) {}

	InvalidURIError::InvalidURIError(const String16& uri) : uri(uri) {}

	static String16 conv3216(const String32& uri) {
		String16 result;
		UTF16Encoder16 utf16;
		Transcode::transcodeString2<Char32, Char16>(uri, result, utf16);
		return result;
	}

	InvalidURIError::InvalidURIError(const String32& uri) : uri(conv3216(uri)) {}

	InvalidURIError::InvalidURIError(const InvalidURIError& error)
			: Error(error), URIError(error), IllegalArgumentError(error), uri(error.uri) {}

	InvalidURIError::~InvalidURIError() {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(InvalidURIError)

}}
