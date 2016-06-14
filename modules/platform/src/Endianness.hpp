#ifndef REDSTRAIN_MOD_PLATFORM_ENDIANNESS_HPP
#define REDSTRAIN_MOD_PLATFORM_ENDIANNESS_HPP

#include <stdint.h>
#include <redstrain/util/StandardIntegerMapping.hpp>

#include "platform.hpp"

#define REDSTRAIN_PLATFORM_SWAB16(x) static_cast<uint16_t>( \
	(static_cast<uint16_t>(x) >> 8) \
	| (static_cast<uint16_t>(x) << 8) \
)
#define REDSTRAIN_PLATFORM_SWAB32(x) static_cast<uint32_t>( \
	(static_cast<uint32_t>(x) >> 24) \
	| ((static_cast<uint32_t>(x) & static_cast<uint32_t>(0x00FF0000u)) >> 8) \
	| ((static_cast<uint32_t>(x) & static_cast<uint32_t>(0x0000FF00u)) << 8) \
	| (static_cast<uint32_t>(x) << 24) \
)
#define REDSTRAIN_PLATFORM_SWAB64(x) static_cast<uint64_t>( \
	(static_cast<uint64_t>(x) >> 56) \
	| ((static_cast<uint64_t>(x) & static_cast<uint64_t>(0x00FF000000000000ull)) >> 40) \
	| ((static_cast<uint64_t>(x) & static_cast<uint64_t>(0x0000FF0000000000ull)) >> 24) \
	| ((static_cast<uint64_t>(x) & static_cast<uint64_t>(0x000000FF00000000ull)) >> 8) \
	| ((static_cast<uint64_t>(x) & static_cast<uint64_t>(0x00000000FF000000ull)) << 8) \
	| ((static_cast<uint64_t>(x) & static_cast<uint64_t>(0x0000000000FF0000ull)) << 24) \
	| ((static_cast<uint64_t>(x) & static_cast<uint64_t>(0x000000000000FF00ull)) << 40) \
	| (static_cast<uint64_t>(x) << 56) \
)

#define REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS8(x) static_cast<uint8_t>(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS8(x) static_cast<uint8_t>(x)
#if REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_LITTLE_ENDIAN
#define REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS16(x) REDSTRAIN_PLATFORM_SWAB16(x)
#define REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS32(x) REDSTRAIN_PLATFORM_SWAB32(x)
#define REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS64(x) REDSTRAIN_PLATFORM_SWAB64(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS16(x) static_cast<uint16_t>(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS32(x) static_cast<uint32_t>(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS64(x) static_cast<uint64_t>(x)
#elif REDSTRAIN_PLATFORM_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
#define REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS16(x) static_cast<uint16_t>(x)
#define REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS32(x) static_cast<uint32_t>(x)
#define REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS64(x) static_cast<uint64_t>(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS16(x) REDSTRAIN_PLATFORM_SWAB16(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS32(x) REDSTRAIN_PLATFORM_SWAB32(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS64(x) REDSTRAIN_PLATFORM_SWAB64(x)
#else
#error REDSTRAIN_PLATFORM_ENDIANNESS must be REDSTRAIN_PLATFORM_LITTLE_ENDIAN or REDSTRAIN_PLATFORM_BIG_ENDIAN
#endif

namespace redengine {
namespace platform {

	template<typename IntegerT>
	class Endianness {

	  private:
		template<int, typename>
		struct Convert;

#define REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(type, bits) \
	template<int Dummy> \
	struct Convert<Dummy, type> { \
		static inline type convertBig(type x) { \
			return static_cast<type>(REDSTRAIN_PLATFORM_CONVERT_BIG_ENDIANNESS ## bits(x)); \
		} \
		static inline type convertLittle(type x) { \
			return static_cast<type>(REDSTRAIN_PLATFORM_CONVERT_LITTLE_ENDIANNESS ## bits(x)); \
		} \
		static inline type swab(type x) { \
			return static_cast<type>(REDSTRAIN_PLATFORM_SWAB ## bits(x)); \
		} \
	};

		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(int8_t, 8)
		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(uint8_t, 8)
		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(int16_t, 16)
		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(uint16_t, 16)
		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(int32_t, 32)
		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(uint32_t, 32)
		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(int64_t, 64)
		REDSTRAIN_PLATFORM_DEFINE_ENDIANNESS_CONVERTER(uint64_t, 64)

		typedef typename util::StandardIntegerMapping<IntegerT>::StandardType StandardType;

	  public:
		static inline IntegerT convertBig(IntegerT x) {
			return Convert<0, StandardType>::convertBig(x);
		}

		static inline IntegerT convertLittle(IntegerT x) {
			return Convert<0, StandardType>::convertLittle(x);
		}

		static inline IntegerT swab(IntegerT x) {
			return Convert<0, StandardType>::swab(x);
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_ENDIANNESS_HPP */
