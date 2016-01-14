#ifndef REDSTRAIN_MOD_UTIL_RANDOM_HPP
#define REDSTRAIN_MOD_UTIL_RANDOM_HPP

#include <cstdlib>

#include "IntegerLog.hpp"
#include "IntegerBits.hpp"
#include "TypeCombinators.hpp"
#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace util {

	REDSTRAIN_UTIL_API void ensureRandomSeeded();

	template<typename IntegerT>
	class RandomIntegerShift {

	  private:
		template<int, bool>
		struct ShiftImpl {
			static inline IntegerT shift(IntegerT current) {
				return current * (static_cast<IntegerT>(RAND_MAX) + static_cast<IntegerT>(1));
			}
		};

		template<int Dummy>
		struct ShiftImpl<Dummy, true> {
			static inline IntegerT shift(IntegerT current) {
				return current;
			}
		};

		typedef typename OfTwoIntegerTypes<IntegerT, long>::WithHigherUpperBound WithHigherUpperBound;

	  public:
		static inline IntegerT shift(IntegerT current) {
			return ShiftImpl<
				0,
				static_cast<WithHigherUpperBound>(RAND_MAX)
						== static_cast<WithHigherUpperBound>(IntegerBounds<IntegerT>::MAX)
			>::shift(current);
		}

	};

	template<typename IntegerT>
	IntegerT randomInt() {
		const unsigned randomBits = static_cast<unsigned>(IntegerLog<long, 2l, RAND_MAX>::EXPONENT) - 1u;
		ensureRandomSeeded();
		IntegerT result = static_cast<IntegerT>(0u);
		unsigned haveBits;
		for(haveBits = 0u; haveBits < static_cast<unsigned>(sizeof(IntegerT)) * 8u; haveBits += randomBits)
			result = static_cast<IntegerT>(RandomIntegerShift<IntegerT>::shift(result)
					+ static_cast<IntegerT>(random()));
		return result;
	}

	template<typename IntegerT>
	void randomInts(IntegerT* sink, MemorySize count) {
		for(; count; --count, ++sink)
			*sink = randomInt<IntegerT>();
	}

	template<typename SpaceT>
	SpaceT randomChoose(SpaceT addressSpace) {
		if(addressSpace <= static_cast<SpaceT>(0))
			return static_cast<SpaceT>(0);
		// Hooo, boy. We can drop whole bits from
		// the random number as to try and force
		// it to be < addressSpace, but we can't
		// drop partial bits; otherwise the result
		// will no longer be evenly distributed.
		// We will therefore construct a bit mask
		// with only the leading zeroes of the
		// addressSpace being unset and the rest
		// set. This mask can then be used to
		// drop as many whole bits as we can.
		//
		// This approach should still beat the
		// "direct" one (without the bit mask)
		// in terms of wasted CPU cycles (for
		// creating the mask first) if addressSpace
		// is much smaller than the actual address
		// space of SpaceT.
		typedef typename IntegerBits<SpaceT>::AsUnsigned USpaceT;
		USpaceT mask = static_cast<USpaceT>(0u);
		while(mask < static_cast<USpaceT>(addressSpace) && mask != IntegerBounds<USpaceT>::MAX)
			mask = static_cast<USpaceT>((mask << 1) | static_cast<USpaceT>(1u));
		for(;;) {
			USpaceT chosen = static_cast<USpaceT>(randomInt<USpaceT>() & mask);
			if(chosen < static_cast<USpaceT>(addressSpace))
				return static_cast<SpaceT>(chosen);
		}
	}

	template<typename SpaceT, typename ChooserT, typename IteratorT>
	void randomChosen(IteratorT begin, IteratorT end, ChooserT chooser, SpaceT addressSpace) {
		for(; begin != end; ++begin)
			*begin = chooser(randomChoose<SpaceT>(addressSpace));
	}

	template<typename CollectionT, typename ElementT, typename SubscriptT>
	class SubscriptRandomChooser {

	  private:
		CollectionT& collection;

	  public:
		SubscriptRandomChooser(CollectionT& collection) : collection(collection) {}
		SubscriptRandomChooser(const SubscriptRandomChooser& chooser) : collection(chooser.collection) {}

		ElementT operator()(SubscriptT subscript) {
			return collection[subscript];
		}

		ElementT operator()(SubscriptT subscript) const {
			return collection[subscript];
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_RANDOM_HPP */
