#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>

#include "InvalidURIPortError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::UTF16Encoder16;

namespace redengine {
namespace vfs {

	InvalidURIPortError::InvalidURIPortError(const string& portRendition)
			: portRendition(Transcode::utf8ToBMP(portRendition)) {}

	InvalidURIPortError::InvalidURIPortError(const String16& portRendition) : portRendition(portRendition) {}

	static String16 conv3216(const String32& portRendition) {
		String16 result;
		UTF16Encoder16 utf16;
		Transcode::transcodeString2<Char32, Char16>(portRendition, result, utf16);
		return result;
	}

	InvalidURIPortError::InvalidURIPortError(const String32& portRendition)
			: portRendition(conv3216(portRendition)) {}

	InvalidURIPortError::InvalidURIPortError(const InvalidURIPortError& error)
			: Error(error), URIError(error), IllegalArgumentError(error), portRendition(error.portRendition) {}

	InvalidURIPortError::~InvalidURIPortError() {}

	REDSTRAIN_DEFINE_ABSTRACT_ERROR(InvalidURIPortError)

}}
