#ifndef REDSTRAIN_MOD_UTIL_FLOATTRAITS_HPP
#define REDSTRAIN_MOD_UTIL_FLOATTRAITS_HPP

#include <list>
#include <cmath>

#include "LeastInteger.hpp"

namespace redengine {
namespace util {

	// ======== FloatLayout ========

	template<size_t AllocationSize>
	class FloatLayout;

	// half precision (binary16)
	template<>
	class FloatLayout<static_cast<size_t>(2u)> {

	  public:
		static const unsigned ALLOCATION_BITS = 16u;
		static const unsigned MANTISSA_BITS = 10u;
		static const unsigned EXPONENT_BITS = 5u;

	};

	// single precision (binary32)
	template<>
	class FloatLayout<static_cast<size_t>(4u)> {

	  public:
		static const unsigned ALLOCATION_BITS = 32u;
		static const unsigned MANTISSA_BITS = 23u;
		static const unsigned EXPONENT_BITS = 8u;

	};

	// double precision (binary64)
	template<>
	class FloatLayout<static_cast<size_t>(8u)> {

	  public:
		static const unsigned ALLOCATION_BITS = 64u;
		static const unsigned MANTISSA_BITS = 52u;
		static const unsigned EXPONENT_BITS = 11u;

	};

	// quadruple precision (binary128)
	template<>
	class FloatLayout<static_cast<size_t>(16u)> {

	  public:
		static const unsigned ALLOCATION_BITS = 128u;
		static const unsigned MANTISSA_BITS = 112u;
		static const unsigned EXPONENT_BITS = 15u;

	};

	// 80 bit extended precision format
	template<>
	class FloatLayout<static_cast<size_t>(12u)> {

	  public:
		static const unsigned ALLOCATION_BITS = 96u;
		static const unsigned MANTISSA_BITS = 64u;
		static const unsigned EXPONENT_BITS = 15u;

	};

	// ======== FloatExponentRange ========

	template<typename FloatT>
	class FloatExponentRange {

	  public:
		static const int EXPONENT_BIAS = (1 << FloatLayout<sizeof(FloatT)>::EXPONENT_BITS) - 1;
		static const int EXPONENT_MIN = -EXPONENT_BIAS + 1;
		static const int EXPONENT_MAX = EXPONENT_BIAS;

	};

	// ======== FloatTraits ========

	template<typename FloatT>
	class FloatTraits;

	#define REDSTRAIN_FLOATTRAITS_IMPL(type, frexp, ldexp, modf) \
		template<> \
		class FloatTraits<type> { \
		  private: \
			typedef FloatLayout<sizeof(type)> Layout; \
			typedef FloatExponentRange<type> Range; \
		  public: \
			typedef LeastInteger<Layout::MANTISSA_BITS + 1u>::Signed Mantissa; \
			typedef LeastInteger<Layout::EXPONENT_BITS + 1u>::Signed Exponent; \
		  public: \
			static const unsigned ALLOCATION_BITS = Layout::ALLOCATION_BITS; \
			static const unsigned MANTISSA_BITS = Layout::MANTISSA_BITS; \
			static const unsigned EXPONENT_BITS = Layout::EXPONENT_BITS; \
			static const int EXPONENT_BIAS = Range::EXPONENT_BIAS; \
			static const int EXPONENT_MIN = Range::EXPONENT_MIN; \
			static const int EXPONENT_MAX = Range::EXPONENT_MAX; \
		  public: \
			static inline type fractionAndExponent(type value, Exponent& exponent) { \
				int exp; \
				type fraction = frexp(value, &exp); \
				exponent = static_cast<Exponent>(exp); \
				return fraction; \
			} \
			static inline type timesTwoToThe(type value, int exponent) { \
				return ldexp(value, exponent); \
			} \
			static inline void integralAndFractional(type value, type& integral, type& fractional) { \
				fractional = modf(value, &integral); \
			} \
			static void mantissaAndExponent(type value, Mantissa& mantissa, Exponent& exponent) { \
				int exp; \
				type src = frexp(value, &exp); \
				mantissa = static_cast<Mantissa>(ldexp(src, static_cast<int>(MANTISSA_BITS))); \
				exponent = static_cast<Exponent>(exp); \
			} \
		};

	REDSTRAIN_FLOATTRAITS_IMPL(float, frexpf, ldexpf, modff)
	REDSTRAIN_FLOATTRAITS_IMPL(double, frexp, ldexp, modf)
	// Ugh, we don't have an integer type that can store the mantissa
	// and sign for an 80-bits-or-wider floating point type: For even
	// the 80 bit 'long double' with 64 bits of mantissa, that would
	// require a primitive integer type with >= 65 bits.
	//REDSTRAIN_FLOATTRAITS_IMPL(long double, frexpl, ldexpl, modfl)

	#undef REDSTRAIN_FLOATTRAITS_IMPL

}}

#endif /* REDSTRAIN_MOD_UTIL_FLOATTRAITS_HPP */
