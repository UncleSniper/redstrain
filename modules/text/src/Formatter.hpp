#ifndef REDSTRAIN_MOD_TEXT_FORMATTER_HPP
#define REDSTRAIN_MOD_TEXT_FORMATTER_HPP

#include <stdint.h>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/util/AddressSpace.hpp>
#include <redstrain/error/ProgrammingError.hpp>

#include "ValueNotFormattableAsNumberError.hpp"
#include "NumberFormatAddressSpaceExceededError.hpp"

namespace redengine {
namespace text {

	template<typename CharT>
	class Formattable {

	  public:
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
		#define REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE \
			default: \
				throw error::ProgrammingError("Unrecognized formattable type: " \
						+ util::StringUtils::toString(static_cast<int>(type)));

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

		/*TODO
		uint16_t asUInt16() const;
		int32_t asInt32() const;
		uint32_t asUInt32() const;
		int64_t asInt64() const;
		uint64_t asUInt64() const;
		float asFloat() const;
		double asDouble() const;
		String asString() const;
		*/

		#undef REDSTRAIN_FORMATTABLE_INT_CONVERSION
		#undef REDSTRAIN_FORMATTABLE_CAST_CONVERSION
		#undef REDSTRAIN_FORMATTABLE_UNKNOWN_TYPE

	};

	template<typename CharT>
	class Formatter {

	  public:
		typedef std::basic_string<CharT> String;

		//TODO

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_FORMATTER_HPP */
