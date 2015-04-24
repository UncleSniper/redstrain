#ifndef REDSTRAIN_MOD_UTIL_INTEGERBITS_HPP
#define REDSTRAIN_MOD_UTIL_INTEGERBITS_HPP

#include <stdint.h>

/**
 * @file
 * Definition of @link redengine::util::IntegerBits integer representation
 * interface@endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Integer representation interface. This class provides
	 * conversions and operations for primitive integral types.
	 *
	 * @tparam IntegerT the primitive integer type on which to
	 * perform operations
	 * @tparconstr
	 * - @c IntegerT must be a primitive integral data type.
	 *
	 * @since 0.1.0
	 */
	template<typename IntegerT>
	class IntegerBits {

	  public:
		/**
		 * Primitive integer type on which to perform operations.
		 * Present so that the type parameter can be extracted from
		 * a template instance.
		 *
		 * @since 0.1.0
		 */
		typedef IntegerT Integer;

	  private:
		template<int, typename IntT>
		struct BitsImpl {};

#define REDSTRAIN_UTIL_INTEGERBITS_IMPL(subject, sig, unsig) \
		template<int Dummy> \
		struct BitsImpl<Dummy, subject> { \
			typedef sig AsSigned; \
			typedef unsig AsUnsigned; \
		};

		REDSTRAIN_UTIL_INTEGERBITS_IMPL(char, char, unsigned char)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(unsigned char, char, unsigned char)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(short, short, unsigned short)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(unsigned short, short, unsigned short)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(int, int, unsigned)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(unsigned, int, unsigned)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(long, long, unsigned long)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(unsigned long, long, unsigned long)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(long long, long long, unsigned long long)
		REDSTRAIN_UTIL_INTEGERBITS_IMPL(unsigned long long, long long, unsigned long long)

		template<typename IntT, bool Signed>
		struct NegativityImpl {

			static inline bool isNegative(IntT) {
				return false;
			}

		};

		template<typename IntT>
		struct NegativityImpl<IntT, true> {

			static inline bool isNegative(IntT value) {
				return value < static_cast<IntT>(0);
			}

		};

		template<typename IntT, bool Signed>
		struct WithSignednessImpl {

			typedef typename BitsImpl<0, IntT>::AsUnsigned As;

		};

		template<typename IntT>
		struct WithSignednessImpl<IntT, true> {

			typedef typename BitsImpl<0, IntT>::AsSigned As;

		};

		typedef BitsImpl<0, IntegerT> Bits;
		static const bool SIGNED = static_cast<IntegerT>(-1) < static_cast<IntegerT>(0);
		typedef NegativityImpl<IntegerT, SIGNED> Negativity;

	  public:
		/**
		 * Signed version of the integer type. That is, the
		 * @c signed counterpart of <tt>IntegerT</tt>. If
		 * @c IntegerT is already signed, @c AsSigned is the
		 * same type as <tt>IntegerT</tt>. For example,
		 * <tt>IntegerBits<unsigned long>::AsSigned</tt>
		 * is defined to be <tt>long</tt>.
		 *
		 * @since 0.1.0
		 */
		typedef typename Bits::AsSigned AsSigned;
		/**
		 * Unsigned version of the integer type. That is, the
		 * @c unsigned counterpart of <tt>IntegerT</tt>. If
		 * @c IntegerT is already unsigned, @c AsUnsigned is the
		 * same type as <tt>IntegerT</tt>. For example,
		 * <tt>IntegerBits<long>::AsUnsigned</tt>
		 * is defined to be <tt>unsigned long</tt>.
		 *
		 * @since 0.1.0
		 */
		typedef typename Bits::AsUnsigned AsUnsigned;

		/**
		 * Provides a version of the integer type with the
		 * specified signedness. See #As for details.
		 *
		 * @tparam Signed whether the provided type should
		 * be a signed type
		 *
		 * @since 0.1.0
		 */
		template<bool Signed>
		struct WithSignedness {

			/**
			 * Version of the integer type with the specified
			 * signedness. That is, #AsSigned if @c Signed or
			 * #AsUnsigned if <tt>!Signed</tt>. For example,
			 * <tt>IntegerBits<unsigned>::WithSignedness<true>::As</tt>
			 * is defined to be <tt>int</tt>.
			 *
			 * @since 0.1.0
			 */
			typedef typename WithSignednessImpl<IntegerT, Signed>::As As;

		};

		/**
		 * Provides a version of the integer type with the
		 * same signedness as the specified type. See #As for
		 * details.
		 *
		 * @tparam TargetT integer type whose signedness
		 * is the desired signedness
		 * @tparconstr
		 * - @c TargetT must be a primitive integral data type.
		 *
		 * @since 0.1.0
		 */
		template<typename TargetT>
		struct WithSameSignednessAs {

			/**
			 * Whether the specified target type is signed.
			 * That is, @c true if @c TargetT is a signed
			 * type or @c false if @c TargetT is an unsigned
			 * type.
			 *
			 * @since 0.1.0
			 */
			static const bool SIGNED = static_cast<TargetT>(-1) < static_cast<TargetT>(0);

			/**
			 * Version of the integer type with the same signedness
			 * as the specified type. That is, #AsSigned if @c TargetT
			 * is signed or #AsUnsigned if @c TargetT is unsigned.
			 * For example,
			 * <tt>IntegerBits<int>::WithSameSignednessAs<unsigned char>::As</tt>
			 * is defined to be <tt>unsigned</tt>.
			 *
			 * @since 0.1.0
			 */
			typedef typename WithSignednessImpl<IntegerT, SIGNED>::As As;

		};

	  private:
		static const unsigned WIDTH = static_cast<unsigned>(sizeof(IntegerT)) * 8u;

	  public:
		/**
		 * Perform left bit shift. Shift the specified @c value to
		 * the left by @c count bits. This function is equivalent
		 * to the C++ @c << operator except that it is well-defined
		 * for all argument values. That is, if @c count is equal to
		 * or greater than the width of @c IntegerT in bits, the
		 * return value is defined to be zero.
		 *
		 * @param value integer value to be shifted
		 * @param count number of bits to shift @c value
		 * @return @c value shifted to the left by @c count bits
		 *
		 * @since 0.1.0
		 */
		static inline IntegerT shiftLeft(IntegerT value, unsigned count) {
			if(count >= WIDTH)
				return static_cast<IntegerT>(0);
			return static_cast<IntegerT>(value << count);
		}

		/**
		 * Perform signed right shift. Shift the specified @c value
		 * to the right by @c count bits, performing sign expansion
		 * if @c IntegerT is a signed type. This function is equivalent
		 * to the C++ @c >> operator except that it is well-defined
		 * for all argument values and has defined semantics on signed
		 * types. That is:
		 * - If @c count is equal to or greater than the width of
		 *   @c IntegerT in bits, the return value is defined to be
		 *   the @c IntegerT value represented by all bits being
		 *   unset or set, if @c value is numerically non-negative
		 *   or negative, respectively.
		 * - Otherwise, the most significant @c count bits of the return
		 *   value are unset or set, if @c value is numerically non-negative
		 *   or negative, respectively.
		 *
		 * @param value integer value to be shifted
		 * @param count number of bits to shift @c value
		 * @return @c value arithmetically shifted to the right by @c bits
		 */
		static IntegerT shiftRightSigned(IntegerT value, unsigned count) {
			if(count >= WIDTH) {
				if(Negativity::isNegative(value))
					return static_cast<IntegerT>(~static_cast<IntegerT>(0));
				else
					return static_cast<IntegerT>(0);
			}
			IntegerT u = static_cast<IntegerT>(static_cast<AsUnsigned>(static_cast<AsUnsigned>(value) >> count));
			if(Negativity::isNegative(value)) {
				AsUnsigned one = static_cast<AsUnsigned>(static_cast<AsUnsigned>(1u) << count);
				IntegerT ones = static_cast<AsUnsigned>(one - static_cast<AsUnsigned>(1u));
				u = static_cast<IntegerT>(u | static_cast<IntegerT>(ones << (WIDTH - count)));
			}
			return u;
		}

		/**
		 * Perform unsigned right bit shift. Shift the specified @c value
		 * to the right by @c count bits, treating a potential sign bit
		 * like a mantissa bit. This function is equivalent
		 * to the C++ @c >> operator except that it is well-defined
		 * for all argument values and has defined semantics on signed
		 * types. That is:
		 * - If @c count is equal to or greater than the width of
		 *   @c IntegerT in bits, the return value is defined to be
		 *   zero.
		 * - Otherwise, the most significant @c count bits of the return
		 *   value are unset.
		 *
		 * @param value integer value to be shifted
		 * @param count number of bits to shift @c value
		 * @return @c value non-arithmetically shifted to the right by @c count bits
		 *
		 * @since 0.1.0
		 */
		static inline IntegerT shiftRightUnsigned(IntegerT value, unsigned count) {
			if(count >= WIDTH)
				return static_cast<IntegerT>(0);
			return static_cast<IntegerT>(static_cast<AsUnsigned>(static_cast<AsUnsigned>(value) >> count));
		}

		/**
		 * Extract a block of bits from an integer. Return the
		 * @c count bit block of @c value with the block index
		 * (from the less significant end) <tt>exponent</tt>.
		 * That is, for @em k in [0, @c count - 1], bit @em k
		 * of the return value equates bit @em k + @c count * @c exponent
		 * of <tt>value</tt>; all other bits in the return value
		 * are unset. For example:
		 * - <tt>IntegerBits<unsigned>::getBits(0x42u, 4u, 0u) == 0x2</tt>
		 * - <tt>IntegerBits<unsigned>::getBits(0x42u, 4u, 1u) == 0x4</tt>
		 *
		 * @param value integer value whose bits to extract
		 * @param count number of bits to extract from @c value
		 * @param exponent index of the bit block to extract
		 * @return the <tt>exponent</tt>-th least significant
		 * @c count bit block of @c value
		 *
		 * @since 0.1.0
		 */
		static IntegerT getBits(IntegerT value, unsigned count, unsigned exponent) {
			AsUnsigned one = static_cast<AsUnsigned>(static_cast<AsUnsigned>(1u) << count);
			IntegerT ones = static_cast<AsUnsigned>(one - static_cast<AsUnsigned>(1u));
			return static_cast<IntegerT>(shiftRightUnsigned(value, exponent * count) & ones);
		}

		/**
		 * Extract a block of bits from an integer and render them
		 * in the specified type. This function is equivalent to
		 * #getBits, except that the return value is converted to
		 * <tt>TargetT</tt>. This conversion is unsigned, meaning
		 * that no sign expansion is performed. For example,
		 * <tt>IntegerBits<char>::getBitsAs<int>('\\xFF', 8u, 0u)</tt>
		 * is 255, not -1 (assuming @c int is wider than @c char).
		 *
		 * @param value integer value whose bits to extract
		 * @param count number of bits to extract from @c value
		 * @param exponent index of the bit block to extract
		 * @return the <tt>exponent</tt>-th least significant
		 * @c count bit block of @c value
		 *
		 * @tparam TargetT desired integer type to render returned
		 * bits in
		 * @tparconstr
		 * - @c TargetT must be a primitive integral data type.
		 *
		 * @since 0.1.0
		 */
		template<typename TargetT>
		static TargetT getBitsAs(IntegerT value, unsigned count, unsigned exponent) {
			AsUnsigned u = static_cast<AsUnsigned>(getBits(value, count, exponent));
			return static_cast<TargetT>(static_cast<typename BitsImpl<0, TargetT>::AsUnsigned>(u));
		}

		/**
		 * Determine whether an integer value is numerically
		 * negative. That is, this function returns
		 * <tt>value < static_cast<IntegerT>(0)</tt>; its purpose
		 * is to avoid the compiler warning that expression is
		 * liable to evoke if @c IntegerT is unsigned.
		 *
		 * @param value number whose sign to check
		 * @return whether @c value is numerically negative
		 *
		 * @since 0.1.0
		 */
		static inline bool isNegative(IntegerT value) {
			return Negativity::isNegative(value);
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_INTEGERBITS_HPP */
