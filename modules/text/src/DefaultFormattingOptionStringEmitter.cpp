#include "DefaultFormattingOptionStringEmitter.hpp"
#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace text {

	DefaultFormattingOptionStringEmitter<char> DefaultFormattingOptionStringEmitter<char>::instance;

	#define c16(c) static_cast<Char16>(c)

	REDSTRAIN_TEXT_API const Char16 DEFAULT_FORMATTING_OPTION_STRING_NAN16[] = {
		c16('N'), c16('a'), c16('N'), c16('\0')
	};

	REDSTRAIN_TEXT_API const Char16 DEFAULT_FORMATTING_OPTION_STRING_POSITIVE_INFINITY16[] = {
		c16('+'), c16('I'), c16('n'), c16('f'), c16('i'), c16('n'), c16('i'), c16('t'), c16('y'), c16('\0')
	};

	REDSTRAIN_TEXT_API const Char16 DEFAULT_FORMATTING_OPTION_STRING_NEGATIVE_INFINITY16[] = {
		c16('-'), c16('I'), c16('n'), c16('f'), c16('i'), c16('n'), c16('i'), c16('t'), c16('y'), c16('\0')
	};

	DefaultFormattingOptionStringEmitter<Char16> DefaultFormattingOptionStringEmitter<Char16>::instance;

}}
