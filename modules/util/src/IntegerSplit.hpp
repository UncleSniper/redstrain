#ifndef REDSTRAIN_MOD_UTIL_INTEGERSPLIT_HPP
#define REDSTRAIN_MOD_UTIL_INTEGERSPLIT_HPP

#include "IntegerBits.hpp"
#include "StandardIntegerMapping.hpp"

namespace redengine {
namespace util {

	template<typename IntegerT>
	class IntegerSplit {

	  public:
		static const unsigned WIDTH = static_cast<unsigned>(sizeof(IntegerT)) * 8u;

	  private:
		template<int, typename T>
		struct SplitImpl {

			typedef typename IntegerBits<T>::AsUnsigned AsUnsigned;

			static inline unsigned countLeadingZeroes(T value) {
				if(!value)
					return WIDTH;
				AsUnsigned v = static_cast<AsUnsigned>(value);
				unsigned size = WIDTH, offset = 0u;
				while((size /= 2u)) {
					if(!(v >> (WIDTH - size))) {
						offset += size;
						v <<= size;
					}
				}
				return offset;
			}

			static inline unsigned countTrailingZeroes(T value) {
				if(!value)
					return WIDTH;
				AsUnsigned v = static_cast<AsUnsigned>(value);
				unsigned size = WIDTH, offset = 0u;
				while((size /= 2u)) {
					if(!(v << (WIDTH - size))) {
						offset += size;
						v >>= size;
					}
				}
				return offset;
			}

		};

#ifdef __GNUC__

		typedef typename StandardIntegerMapping<unsigned>::StandardType UnsignedSIM;
		typedef typename StandardIntegerMapping<int>::StandardType IntSIM;

		template<int Dummy>
		struct SplitImpl<Dummy, UnsignedSIM> {

			static inline unsigned countLeadingZeroes(UnsignedSIM value) {
				return value ? static_cast<unsigned>(__builtin_clz(static_cast<unsigned>(value))) : WIDTH;
			}

			static inline unsigned countTrailingZeroes(UnsignedSIM value) {
				return value ? static_cast<unsigned>(__builtin_ctz(static_cast<unsigned>(value))) : WIDTH;
			}

		};

		template<int Dummy>
		struct SplitImpl<Dummy, IntSIM> {

			static inline unsigned countLeadingZeroes(IntSIM value) {
				return value ? static_cast<unsigned>(__builtin_clz(static_cast<unsigned>(value))) : WIDTH;
			}

			static inline unsigned countTrailingZeroes(IntSIM value) {
				return value ? static_cast<unsigned>(__builtin_ctz(static_cast<unsigned>(value))) : WIDTH;
			}

		};

#endif /* compiler specific implementations */

	  public:
		static unsigned countLeadingZeroes(IntegerT value) {
			return SplitImpl<0, typename StandardIntegerMapping<IntegerT>::StandardType>::countLeadingZeroes(value);
		}

		static unsigned countTrailingZeroes(IntegerT value) {
			return SplitImpl<0, typename StandardIntegerMapping<IntegerT>::StandardType>::countTrailingZeroes(value);
		}

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_INTEGERSPLIT_HPP */
