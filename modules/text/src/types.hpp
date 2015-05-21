#ifndef REDSTRAIN_MOD_TEXT_TYPES_HPP
#define REDSTRAIN_MOD_TEXT_TYPES_HPP

#include <string>
#include <stdint.h>
#include <redstrain/io/parsenum.hpp>

namespace redengine {
namespace text {

	typedef uint16_t Char16;

	typedef std::basic_string<Char16> String16;

	std::string _renderNumber16ForError(const String16&);

}}

namespace redengine {
namespace io {

	template<>
	struct DefaultNumberErrorRenderer<text::Char16> {

		static std::string renderNumberForError(const text::String16& number) {
			return text::_renderNumber16ForError(number);
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_TYPES_HPP */
