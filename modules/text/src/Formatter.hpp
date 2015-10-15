#ifndef REDSTRAIN_MOD_TEXT_FORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTER_HPP

#include <cmath>
#include <stdint.h>
#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/util/IntegerBits.hpp>
#include <redstrain/util/AddressSpace.hpp>
#include <redstrain/util/IntegerBounds.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "ValueNotFormattableAsNumberError.hpp"
#include "NumberFormatAddressSpaceExceededError.hpp"

namespace redengine {
namespace text {

	// ======== DefaultFormattingRendition ========

	template<typename FloatT>
	class FloatingPointIntrospector;

	template<>
	class FloatingPointIntrospector<float> {

	  public:
		typedef int32_t Mantissa;
		typedef int8_t Exponent;

	  public:
		static const unsigned MANTISSA_BITS = 23u;

	  public:
		static void dissect(float value, Mantissa& mantissa, Exponent& exponent) {
			int exp;
			float frc = frexpf(value, &exp);
			// Now we have in 'frc' the plain mantissa of 'value',
			// i.e. a number with the same mantissa, but an exponent
			// of zero. If we just raise that exponent to the number
			// of mantissa bits, this will yield a number that is
			// numerically equal to the value of the actual mantissa
			// bits of 'value'. Hopefully, the system will be smart
			// enough to convert this to an integer by simply copying
			// those bits...
			mantissa = static_cast<Mantissa>(ldexpf(frc, static_cast<int>(MANTISSA_BITS)));
			exponent = static_cast<Exponent>(exp);
		}

	};

	template<>
	class FloatingPointIntrospector<double> {

	  public:
		typedef int64_t Mantissa;
		typedef int16_t Exponent;

	  public:
		static const unsigned MANTISSA_BITS = 52u;

	  public:
		static void dissect(float value, Mantissa& mantissa, Exponent& exponent) {
			// see float version for how this works
			int exp;
			double frc = frexp(value, &exp);
			mantissa = static_cast<Mantissa>(ldexp(frc, static_cast<int>(MANTISSA_BITS)));
			exponent = static_cast<Exponent>(exp);
		}

	};

	// ======== DefaultFormattingRendition ========

	template<typename CharT>
	class DefaultFormattingRendition {

	  public:
		typedef CharT Char;

	  public:
		static const CharT POSITIVE_SIGN = static_cast<CharT>('+');
		static const CharT NEGATIVE_SIGN = static_cast<CharT>('-');
		static const CharT DECIMAL_POINT = static_cast<CharT>('.');
		static const CharT GROUP_SEPARATOR = static_cast<CharT>(',');

	  public:
		static inline CharT digit(unsigned value, bool upperCase) {
			return static_cast<CharT>((upperCase
					? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz")[value]);
		}

	};

	// ======== FormattingOptions ========

	template<
		typename CharT,
		typename RenditionT = DefaultFormattingRendition<CharT>
	>
	class FormattingOptions {

	  public:
		typedef CharT Char;
		typedef RenditionT Rendition;

	  public:
		unsigned base, mantissaWidth, fractionWidth, groupSize;
		bool forcePlus, upperCase;
		CharT padChar, decimalPoint, groupSeparator;

	  public:
		FormattingOptions() : base(10u), mantissaWidth(0u), fractionWidth(1u), groupSize(3u),
				forcePlus(false), upperCase(true), padChar(RenditionT::digit(0u, false)),
				decimalPoint(RenditionT::DECIMAL_POINT), groupSeparator(RenditionT::GROUP_SEPARATOR) {}
		FormattingOptions(unsigned base, unsigned mantissaWidth = 0u, unsigned fractionWidth = 1u,
				bool forcePlus = false, CharT padChar = RenditionT::digit(0u, false), bool upperCase = true,
				CharT decimalPoint = RenditionT::DECIMAL_POINT, CharT groupSeparator = RenditionT::GROUP_SEPARATOR,
				unsigned groupSize = 3u)
				: base(base), mantissaWidth(mantissaWidth), fractionWidth(fractionWidth), groupSize(groupSize),
				forcePlus(forcePlus), upperCase(upperCase), padChar(padChar), decimalPoint(decimalPoint),
				groupSeparator(groupSeparator) {}
		FormattingOptions(const FormattingOptions& options) : base(options.base),
				mantissaWidth(options.mantissaWidth), fractionWidth(options.fractionWidth),
				groupSize(options.groupSize), forcePlus(options.forcePlus), upperCase(options.upperCase),
				padChar(options.padChar), decimalPoint(options.decimalPoint),
				groupSeparator(options.groupSeparator) {}

	};

	// ======== Formattable ========

	template<typename CharT>
	class Formattable {

	  public:
		typedef CharT Char;
		typedef std::basic_string<CharT> String;

		enum Type {
			INT8,
			UINT8,
			INT16,
			UINT16,
			INT32,
			UINT32,
			INT64,
			UINT64,
			FLOAT,
			DOUBLE,
			STRING
		};

		enum Category {
			INTEGRAL,
			FRACTIONAL,
			GRAPHICAL,
			UNKNOWN
		};

	  private:
		Type type;
		union {
			int8_t int8;
			uint8_t uint8;
			int16_t int16;
			uint16_t uint16;
			int32_t int32;
			uint32_t uint32;
			int64_t int64;
			uint64_t uint64;
			float float32;
			double float64;
			String* string;
		} value;

	  public:
		template<typename IntegerT, typename RenditionT>
		static String formatInteger(IntegerT value,
				const FormattingOptions<CharT, RenditionT>& options = FormattingOptions<CharT, RenditionT>()) {
			const unsigned maxDigits = static_cast<unsigned>(util::integerLog<IntegerT>(
				static_cast<IntegerT>(options.base),
				util::IntegerBounds<IntegerT>::MAX
			)) + 1u;
			String result;
			result.reserve(static_cast<typename String::size_type>(options.mantissaWidth > maxDigits
					? options.mantissaWidth : maxDigits));
			bool negative = util::IntegerBits<IntegerT>::isNegative(value);
			unsigned length;
			if(negative) {
				value = -value;
				result += RenditionT::NEGATIVE_SIGN;
				length = 1u;
			}
			else if(options.forcePlus) {
				result += RenditionT::POSITIVE_SIGN;
				length = 1u;
			}
			else
				length = 0u;
			unsigned digits = static_cast<unsigned>(util::integerLog<IntegerT>(
				static_cast<IntegerT>(options.base),
				value
			));
			if(!digits)
				++digits;
			for(; options.mantissaWidth > length + digits; ++length)
				result += options.padChar;
			CharT buffer[digits];
			CharT* insert = buffer + digits;
			if(value) {
				for(; value; value /= static_cast<IntegerT>(options.base)) {
					*--insert = RenditionT::digit(
						static_cast<unsigned>(value % static_cast<IntegerT>(options.base)),
						options.upperCase
					);
				}
			}
			else
				*--insert = RenditionT::digit(0u, options.upperCase);
			result.append(insert, static_cast<typename String::size_type>(digits));
			return result;
		}

		template<typename FloatT, typename RenditionT>
		static String formatFloat(FloatT value,
				const FormattingOptions<CharT, RenditionT>& options = FormattingOptions<CharT, RenditionT>()) {
			typedef FloatingPointIntrospector<FloatT> Introspector;
			typedef typename Introspector::Mantissa Mantissa;
			typedef typename Introspector::Exponent Exponent;
			Mantissa mantissa;
			Exponent exponent;
			Introspector::dissect(value, mantissa, exponent);
			String result;
			//TODO
			return result;
		}

	  public:
		#define REDSTRAIN_FORMATTABLE_CTOR(vtype, vtconst, field) \
			Formattable(vtype value) : type(vtconst) { \
				this->value.field = value; \
			}

		REDSTRAIN_FORMATTABLE_CTOR(int8_t, INT8, int8)
		REDSTRAIN_FORMATTABLE_CTOR(uint8_t, UINT8, uint8)
		REDSTRAIN_FORMATTABLE_CTOR(int16_t, INT16, int16)
		REDSTRAIN_FORMATTABLE_CTOR(uint16_t, UINT16, uint16)
		REDSTRAIN_FORMATTABLE_CTOR(int32_t, INT32, int32)
		REDSTRAIN_FORMATTABLE_CTOR(uint32_t, UINT32, uint32)
		REDSTRAIN_FORMATTABLE_CTOR(int64_t, INT64, int64)
		REDSTRAIN_FORMATTABLE_CTOR(uint64_t, UINT64, uint64)
		REDSTRAIN_FORMATTABLE_CTOR(float, FLOAT, float32)
		REDSTRAIN_FORMATTABLE_CTOR(double, DOUBLE, float64)

		#undef REDSTRAIN_FORMATTABLE_CTOR

		Formattable(const String& value) : type(STRING) {
			this->value.string = new String(value);
		}

		#define REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE \
			default: \
				throw error::ProgrammingError("Unrecognized formattable type: " \
						+ util::StringUtils::toString(static_cast<int>(type)));

		Formattable(const Formattable& formattable) : type(formattable.type) {
			switch(type) {
				case INT8:
					value.int8 = formattable.value.int8;
					break;
				case UINT8:
					value.uint8 = formattable.value.uint8;
					break;
				case INT16:
					value.int16 = formattable.value.int16;
					break;
				case UINT16:
					value.uint16 = formattable.value.uint16;
					break;
				case INT32:
					value.int32 = formattable.value.int32;
					break;
				case UINT32:
					value.uint32 = formattable.value.uint32;
					break;
				case INT64:
					value.int64 = formattable.value.int64;
					break;
				case UINT64:
					value.uint64 = formattable.value.uint64;
					break;
				case FLOAT:
					value.float32 = formattable.value.float32;
					break;
				case DOUBLE:
					value.float64 = formattable.value.float64;
					break;
				case STRING:
					value.string = new String(*formattable.value.string);
					break;
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		~Formattable() {
			if(type == STRING)
				delete value.string;
		}

		inline Type getType() const {
			return type;
		}

		Category getCategory() const {
			switch(type) {
				case INT8:
				case UINT8:
				case INT16:
				case UINT16:
				case INT32:
				case UINT32:
				case INT64:
				case UINT64:
					return INTEGRAL;
				case FLOAT:
				case DOUBLE:
					return FRACTIONAL;
				case STRING:
					return GRAPHICAL;
				default:
					return UNKNOWN;
			}
		}

		#define REDSTRAIN_FORMATTABLE_INT_CONVERSION(vtconst, rettype, vtype, field) \
			case vtconst: \
				if(util::AddressSpace<rettype, vtype>::exceededBy(value.field)) \
					throw NumberFormatAddressSpaceExceededError(); \
				return static_cast<rettype>(value.field);
		#define REDSTRAIN_FORMATTABLE_CAST_CONVERSION(vtconst, rettype, field) \
			case vtconst: \
				return static_cast<rettype>(value.field);

		int8_t asInt8() const {
			switch(type) {
				case INT8: return value.int8;
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT8, int8_t, uint8_t, uint8)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT16, int8_t, int16_t, int16)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT16, int8_t, uint16_t, uint16)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT32, int8_t, int32_t, int32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT32, int8_t, uint32_t, uint32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT64, int8_t, int64_t, int64)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT64, int8_t, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, int8_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, int8_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		uint8_t asUInt8() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT8, uint8_t, int8_t, int8)
				case UINT8: return value.uint8;
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT16, uint8_t, int16_t, int16)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT16, uint8_t, uint16_t, uint16)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT32, uint8_t, int32_t, int32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT32, uint8_t, uint32_t, uint32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT64, uint8_t, int64_t, int64)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT64, uint8_t, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, uint8_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, uint8_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		int16_t asInt16() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT8, int16_t, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, int16_t, uint8)
				case INT16: return value.int16;
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT16, int16_t, uint16_t, uint16)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT32, int16_t, int32_t, int32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT32, int16_t, uint32_t, uint32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT64, int16_t, int64_t, int64)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT64, int16_t, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, int16_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, int16_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		uint16_t asUInt16() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT8, uint16_t, int8_t, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, uint16_t, uint8)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT16, uint16_t, int16_t, int16)
				case UINT16: return value.uint16;
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT32, uint16_t, int32_t, int32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT32, uint16_t, uint32_t, uint32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT64, uint16_t, int64_t, int64)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT64, uint16_t, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, uint16_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, uint16_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		int32_t asInt32() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT8, int32_t, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, int32_t, uint8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT16, int32_t, int16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT16, int32_t, uint16)
				case INT32: return value.int32;
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT32, int32_t, uint32_t, uint32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT64, int32_t, int64_t, int64)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT64, int32_t, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, int32_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, int32_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		uint32_t asUInt32() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT8, uint32_t, int8_t, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, uint32_t, uint8)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT16, uint32_t, int16_t, int16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT16, uint32_t, uint16)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT32, uint32_t, int32_t, int32)
				case UINT32: return value.uint32;
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT64, uint32_t, int64_t, int64)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT64, uint32_t, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, int32_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, int32_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		int64_t asInt64() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT8, int64_t, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, int64_t, uint8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT16, int64_t, int16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT16, int64_t, uint16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT32, int64_t, int32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT32, int64_t, uint32)
				case INT64: return value.int64;
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(UINT64, int64_t, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, int64_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, int64_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		uint64_t asUInt64() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT8, uint64_t, int8_t, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, uint64_t, uint8)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT16, uint64_t, int16_t, int16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT16, uint64_t, uint16)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT32, uint64_t, int32_t, int32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT32, uint64_t, uint32)
				REDSTRAIN_FORMATTABLE_INT_CONVERSION(INT64, uint64_t, int64_t, int64)
				case UINT64: return value.uint64;
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, uint64_t, float32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, uint64_t, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		float asFloat() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT8, float, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, float, uint8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT16, float, int16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT16, float, uint16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT32, float, int32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT32, float, uint32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT64, float, int64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT64, float, uint64)
				case FLOAT: return value.float32;
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(DOUBLE, float, float64)
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		double asDouble() const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT8, double, int8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT8, double, uint8)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT16, double, int16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT16, double, uint16)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT32, double, int32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT32, double, uint32)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(INT64, double, int64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(UINT64, double, uint64)
				REDSTRAIN_FORMATTABLE_CAST_CONVERSION(FLOAT, double, float32)
				case DOUBLE: return value.float64;
				case STRING: throw ValueNotFormattableAsNumberError();
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		#define REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(vtconst, vtype, field) \
			case vtconst: \
				return formatInteger<vtype, RenditionT>(value.field, options);
		#define REDSTRAIN_FORMATTABLE_STRING_FLOAT_CONVERSION(vtconst, vtype, field) \
			case vtconst: \
				return formatFloat<vtype, RenditionT>(value.field, options);

		template<typename RenditionT>
		String asString(const FormattingOptions<CharT, RenditionT>& options
				= FormattingOptions<CharT, RenditionT>()) const {
			switch(type) {
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(INT8, int8_t, int8)
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(UINT8, uint8_t, uint8)
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(INT16, int16_t, int16)
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(UINT16, uint16_t, uint16)
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(INT32, int32_t, int32)
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(UINT32, uint32_t, uint32)
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(INT64, int64_t, int64)
				REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION(UINT64, uint64_t, uint64)
				REDSTRAIN_FORMATTABLE_STRING_FLOAT_CONVERSION(FLOAT, float, float32)
				REDSTRAIN_FORMATTABLE_STRING_FLOAT_CONVERSION(DOUBLE, double, float64)
				case STRING: return *value.string;
				REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
			}
		}

		#undef REDSTRAIN_FORMATTABLE_INT_CONVERSION
		#undef REDSTRAIN_FORMATTABLE_CAST_CONVERSION
		#undef REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE
		#undef REDSTRAIN_FORMATTABLE_STRING_INT_CONVERSION

	};

	// ======== Formatter ========

	template<
		typename CharT,
		typename NumericRenditionT = DefaultFormattingRendition<CharT>
	>
	class Formatter {

	  public:
		typedef CharT Char;
		typedef NumericRenditionT NumericRendition;
		typedef std::basic_string<CharT> String;
		typedef FormattingOptions<CharT, NumericRenditionT> Options;

	  private:
		Options defaultOptions;

	  public:
		Formatter() {}
		Formatter(const Formatter& formatter) : defaultOptions(formatter.defaultOptions) {}

		inline Options& getDefaultFormattingOptions() {
			return defaultOptions;
		}

		inline const Options& getDefaultFormattingOptions() const {
			return defaultOptions;
		}

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTER_HPP */
