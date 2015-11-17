#include <string>
#include <cstdlib>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/platform/platform.hpp>

using std::string;
using redengine::util::StringUtils;

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

	void splitLCMessages(string& locale, string& charset) {
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

	string guessSystemLocale() {
		string locale, charset;
		splitLCMessages(locale, charset);
		return StringUtils::trim(locale);
	}

	string guessTerminalCharset() {
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

}}
