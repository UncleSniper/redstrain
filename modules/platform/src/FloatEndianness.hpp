#ifndef REDSTRAIN_MOD_PLATFORM_FLOATENDIANNESS_HPP
#define REDSTRAIN_MOD_PLATFORM_FLOATENDIANNESS_HPP

#include <cstring>

#include "Endianness.hpp"

#if REDSTRAIN_PLATFORM_FLOAT_ENDIANNESS == REDSTRAIN_PLATFORM_LITTLE_ENDIAN
#define REDSTRAIN_PLATFORM_CONVERT_BIG_FLOAT_ENDIANNESS(itype, x) Endianness<itype>::swab(x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_FLOAT_ENDIANNESS(itype, x) (x)
#elif REDSTRAIN_PLATFORM_FLOAT_ENDIANNESS == REDSTRAIN_PLATFORM_BIG_ENDIAN
#define REDSTRAIN_PLATFORM_CONVERT_BIG_FLOAT_ENDIANNESS(itype, x) (x)
#define REDSTRAIN_PLATFORM_CONVERT_LITTLE_FLOAT_ENDIANNESS(itype, x) Endianness<itype>::swab(x)
#else
#error REDSTRAIN_PLATFORM_FLOAT_ENDIANNESS must be REDSTRAIN_PLATFORM_LITTLE_ENDIAN or REDSTRAIN_PLATFORM_BIG_ENDIAN
#endif

namespace redengine {
namespace platform {

	template<typename FloatT>
	class FloatEndianness {

	  private:
		template<int, typename>
		struct Convert;

#define REDSTRAIN_PLATFORM_DEFINED_FLOAT_ENDIANNESS_CONVERTER(ftype, itype) \
	template<int Dummy> \
	struct Convert<Dummy, ftype> { \
		typedef itype Integer; \
		static itype toNativeBits(ftype x) { \
			itype result; \
			memcpy(&result, &x, sizeof(ftype)); \
			return result; \
		} \
		static itype toBigBits(ftype x) { \
			itype result; \
			memcpy(&result, &x, sizeof(ftype)); \
			return REDSTRAIN_PLATFORM_CONVERT_BIG_FLOAT_ENDIANNESS(itype, result); \
		} \
		static itype toLittleBits(ftype x) { \
			itype result; \
			memcpy(&result, &x, sizeof(ftype)); \
			return REDSTRAIN_PLATFORM_CONVERT_LITTLE_FLOAT_ENDIANNESS(itype, result); \
		} \
		static ftype fromNativeBits(itype x) { \
			ftype result; \
			memcpy(&result, &x, sizeof(ftype)); \
			return result; \
		} \
		static ftype fromBigBits(itype x) { \
			x = REDSTRAIN_PLATFORM_CONVERT_BIG_FLOAT_ENDIANNESS(itype, x); \
			ftype result; \
			memcpy(&result, &x, sizeof(ftype)); \
			return result; \
		} \
		static ftype fromLittleBits(itype x) { \
			x = REDSTRAIN_PLATFORM_CONVERT_LITTLE_FLOAT_ENDIANNESS(itype, x); \
			ftype result; \
			memcpy(&result, &x, sizeof(ftype)); \
			return result; \
		} \
	};

		REDSTRAIN_PLATFORM_DEFINED_FLOAT_ENDIANNESS_CONVERTER(float, util::FloatBits)
		REDSTRAIN_PLATFORM_DEFINED_FLOAT_ENDIANNESS_CONVERTER(double, util::DoubleBits)
		REDSTRAIN_PLATFORM_DEFINED_FLOAT_ENDIANNESS_CONVERTER(long double, util::LongDoubleBits)

	  public:
		typedef typename Convert<0, FloatT>::Integer Bits;

	  public:
		static inline Bits toNativeBits(FloatT x) {
			return Convert<0, FloatT>::toNativeBits(x);
		}

		static inline Bits toBigBits(FloatT x) {
			return Convert<0, FloatT>::toBigBits(x);
		}

		static inline Bits toLittleBits(FloatT x) {
			return Convert<0, FloatT>::toLittleBits(x);
		}

		static inline FloatT fromNativeBits(Bits x) {
			return Convert<0, FloatT>::fromNativeBits(x);
		}

		static inline FloatT fromBigBits(Bits x) {
			return Convert<0, FloatT>::fromBigBits(x);
		}

		static inline FloatT fromLittleBits(Bits x) {
			return Convert<0, FloatT>::fromLittleBits(x);
		}

	};

}}

#endif /* REDSTRAIN_MOD_PLATFORM_FLOATENDIANNESS_HPP */
