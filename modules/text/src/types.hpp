#ifndef REDSTRAIN_MOD_TEXT_TYPES_HPP
#define REDSTRAIN_MOD_TEXT_TYPES_HPP

#include <string>
#include <stdint.h>
#include <redstrain/io/parsenum.hpp>

namespace redengine {
namespace text {

	typedef uint16_t Char16;
	typedef uint32_t Char32;

	typedef std::basic_string<Char16> String16;
	typedef std::basic_string<Char32> String32;

	std::string _renderNumber16ForError(const String16&);
	std::string _renderNumber32ForError(const String32&);

}}

namespace redengine {
namespace io {

	template<>
	struct DefaultNumberErrorRenderer<text::Char16> {

		static std::string renderNumberForError(const text::String16& number) {
			return text::_renderNumber16ForError(number);
		}

	};

	template<>
	struct DefaultNumberErrorRenderer<text::Char32> {

		static std::string renderNumberForError(const text::String32& number) {
			return text::_renderNumber32ForError(number);
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TYPES_HPP */
