#ifndef REDSTRAIN_MOD_UTIL_RANDOM_HPP
#define REDSTRAIN_MOD_UTIL_RANDOM_HPP

#include "IntegerSplit.hpp"
#include "api.hpp"
#include "types.hpp"

namespace redengine {
namespace util {

	class REDSTRAIN_UTIL_API Random {

	  public:
		Random();
		Random(const Random&);
		virtual ~Random();

		virtual MemorySize getSeedSize() const = 0;
		virtual void seed(const char*) = 0;
		virtual void seedFrom(Random&);
		virtual void randomBytes(char*, MemorySize) = 0;

		template<typename IntegerT>
		IntegerT randomInt() {
			IntegerT i;
			randomBytes(reinterpret_cast<char*>(&i), static_cast<MemorySize>(sizeof(IntegerT)));
			return i;
		}

		template<typename IntegerT>
		IntegerT randomInt(IntegerT modulus) {
			if(modulus <= static_cast<IntegerT>(1))
				return static_cast<IntegerT>(0);
			const IntegerT mask = IntegerBits<IntegerT>::shiftRightUnsigned(
				static_cast<IntegerT>(~static_cast<IntegerT>(0)),
				IntegerSplit<IntegerT>::WIDTH - IntegerSplit<IntegerT>::countLeadingZeroes(modulus)
			);
			IntegerT i;
			do {
				randomBytes(reinterpret_cast<char*>(&i), static_cast<MemorySize>(sizeof(IntegerT)));
				i &= mask;
			} while(i >= modulus);
			return i;
		}

		template<typename SpaceT, typename ChooserT, typename IteratorT>
		void randomChosen(IteratorT begin, IteratorT end, ChooserT chooser, SpaceT modulus) {
			for(; begin != end; ++begin)
				*begin = chooser(randomInt<SpaceT>(modulus));
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_RANDOM_HPP */
