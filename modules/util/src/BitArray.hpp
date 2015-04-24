#ifndef REDSTRAIN_MOD_UTIL_BITARRAY_HPP
#define REDSTRAIN_MOD_UTIL_BITARRAY_HPP

#include <new>
#include <cstring>

#include "api.hpp"

namespace redengine {
namespace util {

	class REDSTRAIN_UTIL_API BitArray {

	  private:
		size_t size, tainted;
		unsigned* bits;

		static const size_t ELEMENT_BITS = sizeof(unsigned) * static_cast<size_t>(8u);

		inline void taint(size_t threshold) {
			if(threshold > tainted) {
				memset(bits + tainted, 0, (threshold - tainted) * sizeof(unsigned));
				tainted = threshold;
			}
		}

	  public:
		BitArray(size_t);
		BitArray(const BitArray&);
		~BitArray();

		inline size_t getSize() const {
			return size * ELEMENT_BITS;
		}

		inline void clear() {
			tainted = static_cast<size_t>(0u);
		}

		bool isSet(size_t) const;
		void set(size_t);
		void unset(size_t);
		void set(size_t, bool);
		void toggle(size_t);
		int compare(const BitArray&) const;
		bool isClear() const;

		BitArray& operator=(const BitArray&);
		bool operator==(const BitArray&) const;
		bool operator<(const BitArray&) const;

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_BITARRAY_HPP */
