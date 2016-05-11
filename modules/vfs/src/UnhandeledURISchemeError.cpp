#include <redstrain/text/Transcode.hpp>
#include <redstrain/text/UTF16Encoder16.hpp>

#include "UnhandeledURISchemeError.hpp"

using std::string;
using redengine::text::Char16;
using redengine::text::Char32;
using redengine::text::String16;
using redengine::text::String32;
using redengine::text::Transcode;
using redengine::text::UTF16Encoder16;

namespace redengine {
namespace vfs {

	UnhandeledURISchemeError::UnhandeledURISchemeError(const string& scheme)
			: scheme(Transcode::utf8ToBMP(scheme)) {}

	UnhandeledURISchemeError::UnhandeledURISchemeError(const String16& scheme) : scheme(scheme) {}

	static String16 conv3216(const String32& scheme) {
		String16 result;
		UTF16Encoder16 utf16;
		Transcode::transcodeString2<Char32, Char16>(scheme, result, utf16);
		return result;
	}

	UnhandeledURISchemeError::UnhandeledURISchemeError(const String32& scheme) : scheme(conv3216(scheme)) {}

	UnhandeledURISchemeError::UnhandeledURISchemeError(const UnhandeledURISchemeError& error)
			: Error(error), URIError(error), scheme(error.scheme) {}

	UnhandeledURISchemeError::~UnhandeledURISchemeError() {}

	REDSTRAIN_DEFINE_ERROR(UnhandeledURISchemeError) {
		out << "No handler to parse URI with scheme '" << Transcode::bmpToUTF8(getScheme()) << '\'';
	}

}}
