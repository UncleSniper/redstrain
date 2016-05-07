#ifndef REDSTRAIN_MOD_TEXT_TRANSCODEFORERROR_HPP
#define REDSTRAIN_MOD_TEXT_TRANSCODEFORERROR_HPP

#include "Transcode.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class TranscodeForError;

	template<>
	class TranscodeForError<char> {

	  public:
		static std::string toCharString(const std::string& str) {
			return str;
		}

	};

	template<>
	class TranscodeForError<Char16> {

	  public:
		static std::string toCharString(const String16& str) {
			return Transcode::bmpToUTF8(str);
		}

	};

	template<>
	class TranscodeForError<Char32> {

	  public:
		static std::string toCharString(const String32& str) {
			return Transcode::unicodeToUTF8(str);
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TRANSCODEFORERROR_HPP */
