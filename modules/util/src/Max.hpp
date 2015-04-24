#ifndef REDSTRAIN_MOD_UTIL_MAX_HPP
#define REDSTRAIN_MOD_UTIL_MAX_HPP

/**
 * @file
 * Definition of @link redengine::util::Max compile-time
 * maximum/minimum function@endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Compile-time maximum/minimum function. Provides the
	 * smaller and greater of two integer values as
	 * compile-time constants. See #MAXIMUM and #MININUM.
	 *
	 * @tparam IntegerT type of values to compare
	 * @tparam A first value to compare
	 * @tparam B second value to compare
	 * @tparconstr
	 * - @c IntegerT must be a primitive integral data type.
	 *
	 * @since 0.1.0
	 */
	template<typename IntegerT, IntegerT A, IntegerT B>
	class Max {

	  public:
		/**
		 * Greater of the two values. That is, @c A if
		 * <tt>A > B</tt> or @c B if <tt>A <= B</tt>.
		 *
		 * @since 0.1.0
		 */
		static const IntegerT MAXIMUM = A > B ? A : B;
		/**
		 * Smaller of the two values. That is, @c A if
		 * <tt>A < B</tt> or @c B if <tt>A >= B</tt>.
		 *
		 * @since 0.1.0
		 */
		static const IntegerT MININUM = A < B ? A : B;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MAX_HPP */
