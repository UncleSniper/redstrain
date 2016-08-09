#include <new>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "ParameterizedStringStackOverflowError.hpp"
#include "ParameterizedStringStackUnderflowError.hpp"
#include "UndefinedStringParameterReferencedError.hpp"
#include "IllegalFormattingInParameterizedStringError.hpp"
#include "ParameterizedStringEndsWithinFormattingError.hpp"
#include "tparm.hpp"

using std::string;
using std::bad_alloc;
using std::stringstream;
using redengine::util::StringUtils;
using redengine::error::ProgrammingError;

#define TPARM_STACK_SIZE 32u

namespace redengine {
namespace damnation {

	// ======== TParameter ========

	TParameter::TParameter() : type(INTEGER) {
		value.integer = static_cast<int32_t>(0);
	}

	TParameter::TParameter(int32_t integer) : type(INTEGER) {
		value.integer = integer;
	}

	TParameter::TParameter(const char* str, Allocation alloc) {
		value.string = str;
		switch(alloc) {
			case NEEDS_FREEING:
				type = PRIVATE_STRING;
				break;
			case MUST_NOT_FREE:
				type = FOREIGN_STRING;
				break;
			default:
				throw ProgrammingError("Unrecognized TParameter::Allocation in TParameter::TParameter(): "
						+ StringUtils::toString(static_cast<int>(alloc)));
		}
	}

	TParameter::TParameter(const char* str) : type(FOREIGN_STRING) {
		value.string = str;
	}

	TParameter::TParameter(const string& str) : type(PRIVATE_STRING) {
		char* buffer = (char*)malloc(static_cast<size_t>(str.length()) + static_cast<size_t>(1u));
		if(!buffer)
			throw bad_alloc();
		strcpy(buffer, str.c_str());
		value.string = buffer;
	}

	TParameter::TParameter(const TParameter& parameter) : type(parameter.type) {
		switch(type) {
			case INTEGER:
				value.integer = parameter.value.integer;
				break;
			case FOREIGN_STRING:
				value.string = parameter.value.string;
				break;
			case PRIVATE_STRING:
				if(parameter.value.string) {
					value.string = strdup(parameter.value.string);
					if(!value.string)
						throw bad_alloc();
				}
				else
					value.string = NULL;
				break;
			default:
				throw ProgrammingError("Unrecognized TParameter::Type in TParameter::TParameter(): "
						+ StringUtils::toString(static_cast<int>(type)));
		}
	}

	TParameter::~TParameter() {
		switch(type) {
			case INTEGER:
			case FOREIGN_STRING:
				break;
			case PRIVATE_STRING:
				if(value.string)
					free(const_cast<char*>(value.string));
				break;
			default:
				// Gnnnnnn, we really shouldn't throw in a destructor...
				break;
		}
	}

	TParameter::Allocation TParameter::getAllocation() const {
		switch(type) {
			case INTEGER:
			case FOREIGN_STRING:
				return MUST_NOT_FREE;
			case PRIVATE_STRING:
				return NEEDS_FREEING;
			default:
				throw ProgrammingError("Unrecognized TParameter::Type in TParameter::getAllocation(): "
						+ StringUtils::toString(static_cast<int>(type)));
		}
	}

	int32_t TParameter::intValue() const {
		return type == INTEGER ? value.integer : static_cast<int32_t>(0);
	}

	const char* TParameter::stringValue() const {
		switch(type) {
			case FOREIGN_STRING:
			case PRIVATE_STRING:
				return value.string;
			case INTEGER:
			default:
				return NULL;
		}
	}

	TParameter& TParameter::operator=(const TParameter& other) {
		const char* copy;
		if(other.type == PRIVATE_STRING && other.value.string) {
			copy = strdup(other.value.string);
			if(!copy)
				throw bad_alloc();
		}
		else
			copy = NULL;
		if(type == PRIVATE_STRING)
			free(const_cast<char*>(value.string));
		type = other.type;
		switch(type) {
			case INTEGER:
				value.integer = other.value.integer;
				break;
			case FOREIGN_STRING:
				value.string = other.value.string;
				break;
			case PRIVATE_STRING:
				value.string = copy;
				break;
			default:
				type = INTEGER;
				throw ProgrammingError("Unrecognized TParameter::Type in TParameter::operator=(): "
						+ StringUtils::toString(static_cast<int>(type)));
		}
		return *this;
	}

	TParameter::Ordering TParameter::compare(const TParameter& other) const {
		int cmp;
		switch(type) {
			case INTEGER:
				if(other.type != INTEGER)
					return LESS;
				if(value.integer < other.value.integer)
					return LESS;
				if(value.integer > other.value.integer)
					return GREATER;
				return EQUAL;
			case FOREIGN_STRING:
			case PRIVATE_STRING:
				if(other.type == INTEGER)
					return GREATER;
				if(!value.string)
					return other.value.string ? LESS : EQUAL;
				if(!other.value.string)
					return GREATER;
				cmp = strcmp(value.string, other.value.string);
				return cmp < 0 ? LESS : (cmp > 0 ? GREATER : EQUAL);
			default:
				throw ProgrammingError("Unrecognized TParameter::Type in TParameter::compare(): "
						+ StringUtils::toString(static_cast<int>(type)));
		}
	}

	bool TParameter::truth() const {
		switch(type) {
			case INTEGER:
				return !!value.integer;
			case FOREIGN_STRING:
			case PRIVATE_STRING:
				// I'd like to consider empty, non-NULL strings false, but
				// I guess we'll have to make them true for compatibility
				// with other implementations... Then again, those pretty
				// much make string veracity undefined, anyway. :P
				return !!value.string;
			default:
				throw ProgrammingError("Unrecognized TParameter::Type in TParameter::truth(): "
						+ StringUtils::toString(static_cast<int>(type)));
		}
	}

	void TParameter::increment() {
		if(type == INTEGER)
			++value.integer;
	}

	// ======== termParamArray ========

	struct FakeTParam {

		union Value {
			int32_t i;
			const char* s;
		};

		TParameter::Type type;
		Value value;

	};

	struct TParmVariables {

		FakeTParam space[26];
		TParameter* vars;
		unsigned touched;

		TParmVariables() : touched(0u) {
			vars = reinterpret_cast<TParameter*>(space);
		}

		~TParmVariables() {
			unsigned u;
			for(u = 0u; u < touched; ++u)
				vars[u].~TParameter();
		}

		void touch(unsigned index) {
			for(; touched <= index; ++touched)
				new(vars + touched) TParameter;
		}

	};

	struct TParmStack {

		FakeTParam space[TPARM_STACK_SIZE];
		TParameter* bottom;
		unsigned beyondTop;
		const TParameter *const *const params;
		unsigned paramCount;
		unsigned tcHackOffset, tcHackLength;
		int inc2patchFlags;

		TParmStack(const TParameter *const* params, unsigned paramCount)
				: beyondTop(0u), params(params), paramCount(paramCount), tcHackOffset(0u), tcHackLength(0u),
				inc2patchFlags(0) {
			bottom = reinterpret_cast<TParameter*>(space);
		}

		~TParmStack() {
			while(beyondTop)
				bottom[--beyondTop].~TParameter();
		}

		void termcapHack() {
			if(beyondTop)
				return;
			unsigned u, index;
			for(u = 0u; u < paramCount; ++u) {
				index = paramCount - u - 1u;
				if(!index && inc2patchFlags & 1)
					push(params[0]->intValue() + static_cast<int32_t>(1));
				else if(index == 1u && inc2patchFlags & 2)
					push(params[1]->intValue() + static_cast<int32_t>(1));
				else
					push(*params[index]);
			}
			tcHackOffset = paramCount - 2u;
			tcHackLength = 2u;
			paramCount = 0u;
		}

		TParameter& top() {
			termcapHack();
			if(!beyondTop)
				throw ParameterizedStringStackUnderflowError();
			return bottom[beyondTop - 1u];
		}

		void push(const TParameter& value) {
			if(beyondTop >= TPARM_STACK_SIZE)
				throw ParameterizedStringStackOverflowError();
			new(bottom + beyondTop) TParameter(value);
			++beyondTop;
		}

		void pop() {
			termcapHack();
			if(!beyondTop)
				throw ParameterizedStringStackUnderflowError();
			bottom[--beyondTop].~TParameter();
			if(beyondTop >= tcHackOffset && beyondTop < tcHackOffset + tcHackLength)
				tcHackLength = beyondTop - tcHackOffset;
		}

		void announceTopModified() {
			if(beyondTop >= tcHackOffset && tcHackLength)
				--tcHackLength;
		}

	};

	#define FMTFL_ALTERNATE   001
	#define FMTFL_SIGNED      002
	#define FMTFL_LEFT_ADJUST 004
	#define FMTFL_BLANK       010

	struct TParmFormatOptions {
		int flags;
		unsigned width, precision;
	};

	static unsigned countDigits(int32_t value, int32_t base, int32_t& modulus) {
		unsigned digits = 0u;
		modulus = static_cast<int32_t>(1);
		for(; value; value /= base) {
			++digits;
			modulus *= base;
		}
		return digits;
	}

	static const char *const TPARM_HEX_DIGITS_LOWER = "0123456789abcdef";
	static const char *const TPARM_HEX_DIGITS_UPPER = "0123456789ABCDEF";

	static void doFakePrintf(string& sink, const TParmFormatOptions& options,
			char conversion, const TParameter& value) {
		int32_t base;
		const char* basePrefix;
		const char* hexDigits = TPARM_HEX_DIGITS_LOWER;
		switch(conversion) {
			case 'o':
				base = static_cast<int32_t>(8);
				basePrefix = "0";
				goto numeric;
			case 'X':
				hexDigits = TPARM_HEX_DIGITS_UPPER;
				base = static_cast<int32_t>(16);
				basePrefix = "0X";
				goto numeric;
			case 'x':
				base = static_cast<int32_t>(16);
				basePrefix = "0x";
				goto numeric;
			case 'd':
				base = static_cast<int32_t>(10);
				basePrefix = "";
			numeric:
				{
					if(!(options.flags & FMTFL_ALTERNATE))
						basePrefix = "";
					int32_t val = value.intValue(), modulus;
					unsigned bplen = static_cast<unsigned>(strlen(basePrefix));
					unsigned digits = countDigits(val, base, modulus);
					if(!digits)
						++digits;
					unsigned diglen = options.precision > digits ? options.precision : digits;
					unsigned sglen = options.flags & (FMTFL_SIGNED | FMTFL_BLANK)
							|| val < static_cast<int32_t>(0) ? 1u : 0u;
					unsigned vlen = sglen + bplen + diglen;  // length of field contents ("value")
					unsigned pad = vlen >= options.width ? 0u : options.width - vlen;
					if(!(options.flags & FMTFL_LEFT_ADJUST)) {
						for(; pad; --pad)
							sink += ' ';
					}
					if(val < static_cast<int32_t>(0)) {
						sink += '-';
						val = -val;
					}
					else if(options.flags & FMTFL_SIGNED)
						sink += '+';
					else if(options.flags & FMTFL_BLANK)
						sink += ' ';
					sink += basePrefix;
					for(; diglen > digits; --diglen)
						sink += ' ';
					if(val)
						modulus /= base;
					for(; modulus; modulus /= base) {
						sink += hexDigits[val / modulus];
						val %= modulus;
					}
					for(; pad; --pad)
						sink += ' ';
				}
				break;
			case 's':
				{
					const char* val = value.stringValue();
					unsigned slen;
					if(val)
						slen = static_cast<unsigned>(strlen(val));
					else {
						val = "";
						slen = 0u;
					}
					unsigned vlen = options.precision && options.precision < slen ? options.precision : slen;
					unsigned pad = vlen >= options.width ? 0u : options.width - vlen;
					if(!(options.flags & FMTFL_LEFT_ADJUST)) {
						for(; pad; --pad)
							sink += ' ';
					}
					sink.append(val, static_cast<string::size_type>(vlen));
					for(; pad; --pad)
						sink += ' ';
				}
				break;
			default:
				throw ProgrammingError("Unrecognized conversion in doFakePrintf(): "
						+ string(&conversion, static_cast<string::size_type>(1u)));
		}
	}

	static void binaryOp(char op, TParmStack& stack) {
		TParameter right(stack.top());
		stack.pop();
		int32_t value;
		TParameter& left = stack.top();
		switch(op) {
			case '+':
				left = left.intValue() + right.intValue();
				break;
			case '-':
				left = left.intValue() - right.intValue();
				break;
			case '*':
				left = left.intValue() * right.intValue();
				break;
			case '/':
				value = right.intValue();
				left = value ? left.intValue() / value : static_cast<int32_t>(0);
				break;
			case 'm':
				value = right.intValue();
				left = value ? left.intValue() % value : static_cast<int32_t>(0);
				break;
			case '&':
				left = left.intValue() & right.intValue();
				break;
			case '|':
				left = left.intValue() | right.intValue();
				break;
			case '^':
				left = left.intValue() ^ right.intValue();
				break;
			case '=':
				left = static_cast<int32_t>(left.compare(right) == TParameter::EQUAL);
				break;
			case '>':
				left = static_cast<int32_t>(left.compare(right) == TParameter::GREATER);
				break;
			case '<':
				left = static_cast<int32_t>(left.compare(right) == TParameter::LESS);
				break;
			case 'A':
				left = static_cast<int32_t>(left.truth() && right.truth());
				break;
			case 'O':
				left = static_cast<int32_t>(left.truth() || right.truth());
				break;
			default:
				throw ProgrammingError("Unrecognized binary operator in termParamArray(): "
						+ string(&op, static_cast<string::size_type>(1u)));
		}
		stack.announceTopModified();
	}

	static void unaryOp(char op, TParmStack& stack) {
		TParameter& opnd = stack.top();
		switch(op) {
			case '!':
				opnd = static_cast<int32_t>(!opnd.truth());
				break;
			case '~':
				opnd = ~opnd.intValue();
				break;
			default:
				throw ProgrammingError("Unrecognized binary operator in termParamArray(): "
						+ string(&op, static_cast<string::size_type>(1u)));
		}
		stack.announceTopModified();
	}

	REDSTRAIN_DAMNATION_API void termParamArray(string& sink, const char* format,
			const TParameter *const* params, unsigned paramCount) {
		sink.clear();
		if(!format)
			return;
		size_t flen = strlen(format);
		sink.reserve(static_cast<string::size_type>(flen * static_cast<size_t>(2u)));
		TParmVariables svars, dvars;
		TParmStack stack(params, paramCount);
		TParmFormatOptions options;
		bool didInc2 = false;
		enum {
			FPFS_NONE,
			FPFS_EMPTY_FLAGS,
			FPFS_FLAGS,
			FPFS_WIDTH,
			FPFS_DOT,
			FPFS_PRECISION,
			FPFS_DONE
		} fpfState;
		enum {
			IPS_NONE,
			IPS_SIGN,
			IPS_DIGITS,
			IPS_DONE
		} ipState;
		for(; *format; ++format) {
			if(*format != '%') {
				sink += *format;
				continue;
			}
			if(!*++format)
				throw ParameterizedStringEndsWithinFormattingError();
			switch(*format) {
				case '%':
					sink += '%';
					break;
				case ':':
				case '#':
				/* those two can't actually start the format string; hence the ':'
				case '+':
				case '-':
				*/
				case ' ':
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				case 'd':
				case 'o':
				case 'x':
				case 'X':
				case 's':
					// begin fake printf magic
					options.flags = 0;
					options.width = options.precision = 0u;
					fpfState = FPFS_NONE;
					do {
						if(!*format)
							throw ParameterizedStringEndsWithinFormattingError();
						switch(fpfState) {
							case FPFS_NONE:
								switch(*format) {
									case ':':
										fpfState = FPFS_EMPTY_FLAGS;
										++format;
										break;
									case '#':
									case '+':
									case '-':
									case ' ':
										fpfState = FPFS_FLAGS;
										break;
									case 'd':
									case 'o':
									case 'x':
									case 'X':
									case 's':
										fpfState = FPFS_PRECISION;
										break;
									default:
										if(*format >= '0' && *format <= '9')
											fpfState = FPFS_WIDTH;
										else
											throw IllegalFormattingInParameterizedStringError();
										break;
								}
								break;
							case FPFS_EMPTY_FLAGS:
								switch(*format) {
									case '#':
									case '+':
									case '-':
									case ' ':
										fpfState = FPFS_FLAGS;
										break;
									default:
										throw IllegalFormattingInParameterizedStringError();
								}
								break;
							case FPFS_FLAGS:
								switch(*format) {
									#define clamp(c, fl) \
										case c: \
											options.flags |= fl; \
											++format; \
											break;
									clamp('#', FMTFL_ALTERNATE)
									clamp('+', FMTFL_SIGNED)
									clamp('-', FMTFL_LEFT_ADJUST)
									clamp(' ', FMTFL_BLANK)
									#undef clamp
									case 'd':
									case 'o':
									case 'x':
									case 'X':
									case 's':
										fpfState = FPFS_PRECISION;
										break;
									default:
										if(*format >= '0' && *format <= '9')
											fpfState = FPFS_WIDTH;
										else
											throw IllegalFormattingInParameterizedStringError();
										break;
								}
								break;
							case FPFS_WIDTH:
								switch(*format) {
									case '.':
										fpfState = FPFS_DOT;
										++format;
										break;
									case 'd':
									case 'o':
									case 'x':
									case 'X':
									case 's':
										fpfState = FPFS_PRECISION;
										break;
									default:
										if(*format >= '0' && *format <= '9') {
											options.width = options.width * 10u
													+ static_cast<unsigned>(*format - '0');
											++format;
										}
										else
											throw IllegalFormattingInParameterizedStringError();
										break;
								}
								break;
							case FPFS_DOT:
								if(*format >= '0' && *format <= '9')
									fpfState = FPFS_PRECISION;
								else
									throw IllegalFormattingInParameterizedStringError();
								break;
							case FPFS_PRECISION:
								switch(*format) {
									case 'd':
									case 'o':
									case 'x':
									case 'X':
									case 's':
										doFakePrintf(sink, options, *format, stack.top());
										stack.pop();
										fpfState = FPFS_DONE;
										break;
									default:
										if(*format >= '0' && *format <= '9') {
											options.precision = options.precision * 10u
													+ static_cast<unsigned>(*format - '0');
											++format;
										}
										else
											throw IllegalFormattingInParameterizedStringError();
										break;
								}
								break;
							default:
								throw ProgrammingError("Unrecognized fpfState in termParamArray(): "
										+ StringUtils::toString(static_cast<int>(fpfState)));
						}
					} while(fpfState != FPFS_DONE);
					// end fake printf magic
					break;
				case 'c':
					sink += static_cast<char>(static_cast<unsigned char>(
							static_cast<uint32_t>(stack.top().intValue())));
					stack.pop();
					break;
				case 'p':
					if(!*++format)
						throw ParameterizedStringEndsWithinFormattingError();
					if(*format <= '1' || *format >= '9')
						throw IllegalFormattingInParameterizedStringError();
					{
						unsigned index = static_cast<unsigned>(*format - '1');
						if(index >= paramCount)
							throw UndefinedStringParameterReferencedError(index);
						if(!index && stack.inc2patchFlags & 1)
							stack.push(params[0]->intValue() + static_cast<int32_t>(1));
						else if(index == 1u && stack.inc2patchFlags & 2)
							stack.push(params[1]->intValue() + static_cast<int32_t>(1));
						else
							stack.push(*params[index]);
					}
					break;
				case 'P':
					if(!*++format)
						throw ParameterizedStringEndsWithinFormattingError();
					if(*format >= 'a' && *format <= 'z') {
						unsigned index = static_cast<unsigned>(*format - 'a');
						dvars.touch(index);
						dvars.vars[index] = stack.top();
						stack.pop();
					}
					else if(*format >= 'A' && *format <= 'Z') {
						unsigned index = static_cast<unsigned>(*format - 'A');
						svars.touch(index);
						svars.vars[index] = stack.top();
						stack.pop();
					}
					else
						throw IllegalFormattingInParameterizedStringError();
					break;
				case 'g':
					if(!*++format)
						throw ParameterizedStringEndsWithinFormattingError();
					if(*format >= 'a' && *format <= 'z') {
						unsigned index = static_cast<unsigned>(*format - 'a');
						dvars.touch(index);
						stack.push(dvars.vars[index]);
					}
					else if(*format >= 'A' && *format <= 'Z') {
						unsigned index = static_cast<unsigned>(*format - 'A');
						svars.touch(index);
						stack.push(svars.vars[index]);
					}
					else
						throw IllegalFormattingInParameterizedStringError();
					break;
				case '\'':
					if(!*++format)
						throw ParameterizedStringEndsWithinFormattingError();
					if(!*++format)
						throw ParameterizedStringEndsWithinFormattingError();
					if(*format != '\'')
						throw IllegalFormattingInParameterizedStringError();
					stack.push(static_cast<int32_t>(static_cast<uint32_t>(static_cast<unsigned char>(format[-1]))));
					break;
				case '{':
					{
						ipState = IPS_NONE;
						bool negate = false;
						int32_t value;
						do {
							if(!*++format)
								throw ParameterizedStringEndsWithinFormattingError();
							switch(ipState) {
								case IPS_NONE:
									switch(*format) {
										case '-':
											negate = true;
										case '+':
											ipState = IPS_SIGN;
											break;
										default:
											if(*format >= '0' && *format <= '9') {
												value = static_cast<int32_t>(*format - '0');
												ipState = IPS_DIGITS;
											}
											else
												throw IllegalFormattingInParameterizedStringError();
									}
									break;
								case IPS_SIGN:
									if(*format >= '0' && *format <= '9') {
										value = static_cast<int32_t>(*format - '0');
										ipState = IPS_DIGITS;
									}
									else
										throw IllegalFormattingInParameterizedStringError();
									break;
								case IPS_DIGITS:
									if(*format == '}')
										ipState = IPS_DONE;
									else if(*format >= '0' && *format <= '9')
										value = value * static_cast<int32_t>(10)
												+ static_cast<int32_t>(*format - '0');
									else
										throw IllegalFormattingInParameterizedStringError();
									break;
								default:
									throw ProgrammingError("Unrecognized ipState in termParamArray(): "
											+ StringUtils::toString(static_cast<int>(fpfState)));
							}
						} while(ipState != IPS_NONE);
						stack.push(negate ? -value : value);
					}
					break;
				case 'l':
					{
						const char* value = stack.top().stringValue();
						stack.top() = value ? static_cast<int32_t>(strlen(value)) : static_cast<int32_t>(0);
						stack.announceTopModified();
					}
					break;
				case '+':
				case '-':
				case '*':
				case '/':
				case 'm':
				case '&':
				case '|':
				case '^':
				case '=':
				case '>':
				case '<':
				case 'A':
				case 'O':
					binaryOp(*format, stack);
					break;
				case '!':
				case '~':
					unaryOp(*format, stack);
					break;
				case 'i':
					if(!didInc2 && paramCount >= 2u) {
						if(params[1]->getType() == TParameter::INTEGER) {
							stack.inc2patchFlags |= 2;
							if(stack.tcHackLength >= 2u)
								stack.bottom[stack.tcHackOffset + 1u].increment();
						}
						if(params[0]->getType() == TParameter::INTEGER) {
							stack.inc2patchFlags |= 1;
							if(stack.tcHackLength >= 1u)
								stack.bottom[stack.tcHackOffset].increment();
						}
						didInc2 = true;
					}
					break;
				default:
					throw IllegalFormattingInParameterizedStringError();
			}
		}
	}

	// ======== termParamSink ========

	REDSTRAIN_DAMNATION_API void termParamSink(string& sink, const char* format) {
		termParamArray(sink, format, NULL, 0u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0
	) {
		const TParameter *const array[] = {
			&param0
		};
		termParamArray(sink, format, array, 1u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1
	) {
		const TParameter *const array[] = {
			&param0,
			&param1
		};
		termParamArray(sink, format, array, 2u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2
	) {
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2
		};
		termParamArray(sink, format, array, 3u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3
	) {
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3
		};
		termParamArray(sink, format, array, 4u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4
	) {
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4
		};
		termParamArray(sink, format, array, 5u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5
	) {
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5
		};
		termParamArray(sink, format, array, 6u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5,
		const TParameter& param6
	) {
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5,
			&param6
		};
		termParamArray(sink, format, array, 7u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5,
		const TParameter& param6,
		const TParameter& param7
	) {
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5,
			&param6,
			&param7
		};
		termParamArray(sink, format, array, 8u);
	}

	REDSTRAIN_DAMNATION_API void termParamSink(
		string& sink, const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5,
		const TParameter& param6,
		const TParameter& param7,
		const TParameter& param8
	) {
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5,
			&param6,
			&param7,
			&param8
		};
		termParamArray(sink, format, array, 9u);
	}

	// ======== termParam ========

	REDSTRAIN_DAMNATION_API string termParam(const char* format) {
		string result;
		termParamArray(result, format, NULL, 0u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0
	) {
		string result;
		const TParameter *const array[] = {
			&param0
		};
		termParamArray(result, format, array, 1u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1
		};
		termParamArray(result, format, array, 2u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2
		};
		termParamArray(result, format, array, 3u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3
		};
		termParamArray(result, format, array, 4u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4
		};
		termParamArray(result, format, array, 5u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5
		};
		termParamArray(result, format, array, 6u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5,
		const TParameter& param6
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5,
			&param6
		};
		termParamArray(result, format, array, 7u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5,
		const TParameter& param6,
		const TParameter& param7
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5,
			&param6,
			&param7
		};
		termParamArray(result, format, array, 8u);
		return result;
	}

	REDSTRAIN_DAMNATION_API string termParam(
		const char* format,
		const TParameter& param0,
		const TParameter& param1,
		const TParameter& param2,
		const TParameter& param3,
		const TParameter& param4,
		const TParameter& param5,
		const TParameter& param6,
		const TParameter& param7,
		const TParameter& param8
	) {
		string result;
		const TParameter *const array[] = {
			&param0,
			&param1,
			&param2,
			&param3,
			&param4,
			&param5,
			&param6,
			&param7,
			&param8
		};
		termParamArray(result, format, array, 9u);
		return result;
	}

}}
