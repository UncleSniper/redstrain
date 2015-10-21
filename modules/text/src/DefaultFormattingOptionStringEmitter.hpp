#ifndef REDSTRAIN_MOD_TEXT_DEFAULTFORMATTINGOPTIONSTRINGEMITTER_HPP
#define REDSTRAIN_MOD_TEXT_DEFAULTFORMATTINGOPTIONSTRINGEMITTER_HPP

#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "FormattingOptionStringEmitter.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class DefaultFormattingOptionStringEmitter;

	template<>
	class DefaultFormattingOptionStringEmitter<char> : public FormattingOptionStringEmitter<char> {

	  public:
		DefaultFormattingOptionStringEmitter() {}

		DefaultFormattingOptionStringEmitter(const DefaultFormattingOptionStringEmitter& emitter)
				: FormattingOptionStringEmitter<char>(emitter) {}

		virtual std::string emitString(FormattingStringOption option) {
			switch(option) {
				case FSO_NAN:
					return "NaN";
				case FSO_POSITIVE_INFINITY:
					return "+Infinity";
				case FSO_NEGATIVE_INFINITY:
					return "-Infinity";
				default:
					throw error::ProgrammingError("Unrecognized formatting string option in "
							"DefaultFormattingOptionStringEmitter::emitString(): "
							+ util::StringUtils::toString(static_cast<int>(option)));
			}
		}

	  public:
		static DefaultFormattingOptionStringEmitter<char> instance;

	};

	REDSTRAIN_TEXT_API extern const Char16 DEFAULT_FORMATTING_OPTION_STRING_NAN16[];
	REDSTRAIN_TEXT_API extern const Char16 DEFAULT_FORMATTING_OPTION_STRING_POSITIVE_INFINITY16[];
	REDSTRAIN_TEXT_API extern const Char16 DEFAULT_FORMATTING_OPTION_STRING_NEGATIVE_INFINITY16[];

	template<>
	class DefaultFormattingOptionStringEmitter<Char16> : public FormattingOptionStringEmitter<Char16> {

	  public:
		DefaultFormattingOptionStringEmitter() {}

		DefaultFormattingOptionStringEmitter(const DefaultFormattingOptionStringEmitter& emitter)
				: FormattingOptionStringEmitter<Char16>(emitter) {}

		virtual String16 emitString(FormattingStringOption option) {
			switch(option) {
				case FSO_NAN:
					return DEFAULT_FORMATTING_OPTION_STRING_NAN16;
				case FSO_POSITIVE_INFINITY:
					return DEFAULT_FORMATTING_OPTION_STRING_POSITIVE_INFINITY16;
				case FSO_NEGATIVE_INFINITY:
					return DEFAULT_FORMATTING_OPTION_STRING_NEGATIVE_INFINITY16;
				default:
					throw error::ProgrammingError("Unrecognized formatting string option in "
							"DefaultFormattingOptionStringEmitter::emitString(): "
							+ util::StringUtils::toString(static_cast<int>(option)));
			}
		}

	  public:
		static DefaultFormattingOptionStringEmitter<Char16> instance;

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_DEFAULTFORMATTINGOPTIONSTRINGEMITTER_HPP */