#include <string>
#include <cstdlib>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/io/StringOutputStream.hpp>
#include <redstrain/platform/platform.hpp>

#include "agnostic.hpp"

using std::string;
using redengine::error::Error;
using redengine::locale::Locale;
using redengine::util::StringUtils;
using redengine::io::StringOutputStream;

namespace redengine {
namespace text {

#if REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_UNIX

	static const char* getLCMessages() {
		const char* value = getenv("LC_ALL");
		if(!value || !*value)
			value = getenv("LC_MESSAGES");
		if(!value || !*value)
			value = getenv("LANG");
		return value && *value ? value : NULL;
	}

	static void splitLCMessages(string& locale, string& charset) {
		const char* lcMessages = getLCMessages();
		if(!lcMessages)
			return;
		string s(lcMessages);
		string::size_type pos = s.find('.');
		if(pos != string::npos) {
			locale = s.substr(static_cast<string::size_type>(0u), pos);
			charset = s.substr(pos + static_cast<string::size_type>(1u));
		}
		else
			locale = s;
	}

	REDSTRAIN_TEXT_API string guessSystemLocale() {
		string locale, charset;
		splitLCMessages(locale, charset);
		locale = StringUtils::trim(locale);
		if(locale == "C")
			return string();
		return locale;
	}

	REDSTRAIN_TEXT_API string guessTerminalCharset() {
		string locale, charset;
		splitLCMessages(locale, charset);
		charset = StringUtils::trim(charset);
		if(charset.empty())
			charset = "UTF-8";
		return charset;
	}

#elif REDSTRAIN_PLATFORM_OS == REDSTRAIN_PLATFORM_OS_WINDOWS

#error TODO: support Windows

#else /* OS not implemented */
#error Platform not supported
#endif /* OS-specific implementations */

	REDSTRAIN_TEXT_API String16 getErrorMessage(const Error& error, const Locale& locale) {
		String16 result;
		StringOutputStream<Char16> stream(result);
		StreamSink16 sink(stream);
		error.printMessage(locale, sink);
		return result;
	}

}}
