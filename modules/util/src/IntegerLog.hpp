#ifndef REDSTRAIN_MOD_UTIL_INTEGERLOG_HPP
#define REDSTRAIN_MOD_UTIL_INTEGERLOG_HPP

#include <stdint.h>

#include "IntegerBounds.hpp"

/**
 * @file
 * Definition of @link redengine::util::IntegerLog compile-time
 * logarithm function@endlink.
 *
 * @since 0.1.0
 */

namespace redengine {
namespace util {

	/**
	 * Compile-time discrete logarithm function. Provides the
	 * logarithm of an integer value to the base of another
	 * as a compile-time constant. See #EXPONENT.
	 *
	 * @tparam IntegerT type of logarith function operands
	 * @tparam Base base of logarithm
	 * @tparam Power value whose logarithm to the given @c Base
	 * to compute
	 * @tparconstr
	 * - @c IntegerT must be a primitive integral data type.
	 * - @c Base must be > 1 or < -1.
	 *
	 * @since 0.1.0
	 */
	template<typename IntegerT, IntegerT Base, IntegerT Power>
	class IntegerLog {

		/* GCC Bug -- we can't do it properly
	  private:
		template<IntegerT RemainingPower, IntegerT Exponent>
		struct Log {

			static const IntegerT EXPONENT = Log<
				static_cast<IntegerT>(RemainingPower / Base),
				static_cast<IntegerT>(Exponent + static_cast<IntegerT>(1))
			>::EXPONENT;

		};

		template<IntegerT Exponent>
		struct Log<static_cast<IntegerT>(0), Exponent> {

			static const IntegerT EXPONENT = Exponent;

		};
		*/

	  private:
		template<int64_t RemainingPower, unsigned Exponent>
		struct SignedLog {

			static const unsigned EXPONENT = SignedLog<
				RemainingPower / static_cast<int64_t>(Base),
				Exponent + 1u
			>::EXPONENT;

		};

		template<unsigned Exponent>
		struct SignedLog<static_cast<int64_t>(0), Exponent> {

			static const unsigned EXPONENT = Exponent;

		};

		template<uint64_t RemainingPower, unsigned Exponent>
		struct UnsignedLog {

			static const unsigned EXPONENT = UnsignedLog<
				RemainingPower / static_cast<uint64_t>(Base),
				Exponent + 1u
			>::EXPONENT;

		};

		template<unsigned Exponent>
		struct UnsignedLog<static_cast<uint64_t>(0u), Exponent> {

			static const unsigned EXPONENT = Exponent;

		};

	  public:
		//static const IntegerT EXPONENT = Log<Power, static_cast<IntegerT>(0)>::EXPONENT;
		/**
		 * Ceiling of desired logarithm. In particular,
		 * <tt>EXPONENT == ceil(log(abs(Base), abs(Power) + 1))</tt>.
		 * The typical interpretations are:
		 * - @c Power must be successively integrally divided
		 *   by @c Base at least @c EXPONENT time to arrive
		 *   at zero. (This is the definition of this constant.)
		 * - Rendering @c Power in the place-value system with
		 *   the given @c Base yields a true <tt>EXPONENT</tt>-digit
		 *   number. (This is the primary purpose of this class.)
		 *
		 * @since 0.1.0
		 */
		static const IntegerT EXPONENT = IntegerBounds<IntegerT>::SIGNED
				? static_cast<IntegerT>(SignedLog<static_cast<int64_t>(Power), 0u>::EXPONENT)
				: static_cast<IntegerT>(UnsignedLog<static_cast<uint64_t>(Power), 0u>::EXPONENT);

	};

	template<typename IntegerT>
	IntegerT integerLog(IntegerT base, IntegerT power) {
		IntegerT exponent = static_cast<IntegerT>(0);
		while(power) {
			power /= base;
			++exponent;
		}
		return exponent;
	}

}}

#endif /* REDSTRAIN_MOD_UTIL_INTEGERLOG_HPP */
