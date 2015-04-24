#ifndef REDSTRAIN_MOD_UTIL_INTEGERBOUNDS_HPP
#define REDSTRAIN_MOD_UTIL_INTEGERBOUNDS_HPP

/**
 * @file
 * Definition of @link redengine::util::IntegerBounds integer type
 * value ranges @endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Integer type value ranges. This class computes the bounds of
	 * primitive integer types, i.e. the minimum and maximum values
	 * of such a type. In this respect, it is similar to
	 * <tt>std::numeric_limits</tt>, except that the bounds are
	 * provided as compile-time constants.
	 *
	 * @tparam IntegerT the primitive integer type whose bounds to compute
	 * @tparconstr
	 * - @c IntegerT must be a primitive integral data type.
	 *
	 * @since 0.1.0
	 */
	template<typename IntegerT>
	class IntegerBounds {

	  private:
		template<typename IntT, bool Signed>
		struct BoundsImpl {

			static const IntT MIN = static_cast<IntT>(0);
			static const IntT MAX = ~static_cast<IntT>(0);

		};

		template<typename IntT>
		struct BoundsImpl<IntT, true> {

			static const IntT MIN = static_cast<IntT>(static_cast<IntT>(1)
					<< (static_cast<unsigned>(sizeof(IntT)) * 8u - 1u));
			static const IntT MAX = ~static_cast<IntT>(static_cast<IntT>(1)
					<< (static_cast<unsigned>(sizeof(IntT)) * 8u - 1u));

		};

	  public:
		/**
		 * Primitive integer type whose bounds to compute. Present so
		 * that the type parameter can be extracted from a template
		 * instance.
		 *
		 * @since 0.1.0
		 */
		typedef IntegerT Integer;

	  public:
		/**
		 * Signedness of the integer type. This is @c true if and only
		 * if @c IntegerT is a signed type.
		 */
		static const bool SIGNED = static_cast<IntegerT>(-1) < static_cast<IntegerT>(0);

	  private:
		typedef BoundsImpl<IntegerT, SIGNED> Bounds;

	  public:
		/**
		 * Minimum value of the integer type. That is, the numerically
		 * lowest possible instance of <tt>IntegerT</tt>.
		 *
		 * @since 0.1.0
		 */
		static const IntegerT MIN = Bounds::MIN;
		/**
		 * Maximum value of the integer type. That is, the numerically
		 * highest possible instance of <tt>IntegerT</tt>.
		 *
		 * @since 0.1.0
		 */
		static const IntegerT MAX = Bounds::MAX;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_INTEGERBOUNDS_HPP */
