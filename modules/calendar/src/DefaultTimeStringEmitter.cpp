#include "DefaultTimeStringEmitter.hpp"

using redengine::text::Char16;
using redengine::text::Char32;

namespace redengine {
namespace calendar {

	DefaultTimeStringEmitter<char> DefaultTimeStringEmitter<char>::instance;

	DefaultTimeStringEmitter<Char16> DefaultTimeStringEmitter<Char16>::instance;

	DefaultTimeStringEmitter<Char32> DefaultTimeStringEmitter<Char32>::instance;

#define c16(c) static_cast<Char16>(c)
#define c32(c) static_cast<Char32>(c)

#define makeArrays(bits, chr) \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _MON[] = { \
		chr('M'), chr('o'), chr('n'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _TUE[] = { \
		chr('T'), chr('u'), chr('e'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _WED[] = { \
		chr('W'), chr('e'), chr('d'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _THU[] = { \
		chr('T'), chr('h'), chr('u'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _FRI[] = { \
		chr('F'), chr('r'), chr('i'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _SAT[] = { \
		chr('S'), chr('a'), chr('t'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _SUN[] = { \
		chr('S'), chr('u'), chr('n'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _MONDAY[] = { \
		chr('M'), chr('o'), chr('n'), chr('d'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _TUESDAY[] = { \
		chr('T'), chr('u'), chr('e'), chr('s'), chr('d'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _WEDNESDAY[] = { \
		chr('W'), chr('e'), chr('d'), chr('n'), chr('e'), chr('s'), chr('d'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _THURSDAY[] = { \
		chr('T'), chr('h'), chr('u'), chr('r'), chr('s'), chr('d'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _FRIDAY[] = { \
		chr('F'), chr('r'), chr('i'), chr('d'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _SATURDAY[] = { \
		chr('S'), chr('a'), chr('t'), chr('u'), chr('r'), chr('d'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _SUNDAY[] = { \
		chr('S'), chr('u'), chr('n'), chr('d'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _JAN[] = { \
		chr('J'), chr('a'), chr('n'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _FEB[] = { \
		chr('F'), chr('e'), chr('b'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _MAR[] = { \
		chr('M'), chr('a'), chr('r'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _APR[] = { \
		chr('A'), chr('p'), chr('r'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _MAY_S[] = { \
		chr('M'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _JUN[] = { \
		chr('J'), chr('u'), chr('n'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _JUL[] = { \
		chr('J'), chr('u'), chr('l'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _AUG[] = { \
		chr('A'), chr('u'), chr('g'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _SEP[] = { \
		chr('S'), chr('e'), chr('p'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _OCT[] = { \
		chr('O'), chr('c'), chr('t'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _NOV[] = { \
		chr('N'), chr('o'), chr('v'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _DEC[] = { \
		chr('D'), chr('e'), chr('c'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _JANUARY[] = { \
		chr('J'), chr('a'), chr('n'), chr('u'), chr('a'), chr('r'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _FEBRUARY[] = { \
		chr('F'), chr('e'), chr('b'), chr('r'), chr('u'), chr('a'), chr('r'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _MARCH[] = { \
		chr('M'), chr('a'), chr('r'), chr('c'), chr('h'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _APRIL[] = { \
		chr('A'), chr('p'), chr('r'), chr('i'), chr('l'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _MAY[] = { \
		chr('M'), chr('a'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _JUNE[] = { \
		chr('J'), chr('u'), chr('n'), chr('e'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _JULY[] = { \
		chr('J'), chr('u'), chr('l'), chr('y'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _AUGUST[] = { \
		chr('A'), chr('u'), chr('g'), chr('u'), chr('s'), chr('t'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _SEPTEMBER[] = { \
		chr('S'), chr('e'), chr('p'), chr('t'), chr('e'), chr('m'), chr('b'), chr('e'), chr('r'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _OCTOBER[] = { \
		chr('O'), chr('c'), chr('t'), chr('o'), chr('b'), chr('e'), chr('r'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _NOVEMBER[] = { \
		chr('N'), chr('o'), chr('v'), chr('e'), chr('m'), chr('b'), chr('e'), chr('r'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _DECEMBER[] = { \
		chr('D'), chr('e'), chr('c'), chr('e'), chr('m'), chr('b'), chr('e'), chr('r'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _AM[] = { \
		chr('A'), chr('M'), chr('\0') \
	}; \
	REDSTRAIN_CALENDAR_API const Char ## bits DEFAULT_TIME_OPTION_STRING ## bits ## _PM[] = { \
		chr('P'), chr('M'), chr('\0') \
	};

	makeArrays(16, c16)
	makeArrays(32, c32)

}}
