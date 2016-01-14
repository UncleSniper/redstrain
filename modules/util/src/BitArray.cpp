#include "BitArray.hpp"

namespace redengine {
namespace util {

	BitArray::BitArray(MemorySize sizeInBits)
			: size((sizeInBits + (ELEMENT_BITS - static_cast<MemorySize>(1u))) / ELEMENT_BITS),
			tainted(static_cast<MemorySize>(0u)), bits(size ? new unsigned[size] : NULL) {}

	BitArray::BitArray(const BitArray& array) : size(array.size), tainted(array.tainted),
			bits(size ? new unsigned[size] : NULL) {
		if(tainted)
			memcpy(bits, array.bits, tainted * sizeof(unsigned));
	}

	BitArray::~BitArray() {
		if(bits)
			delete bits;
	}

	bool BitArray::isSet(MemorySize index) const {
		if(index < tainted)
			return !!(bits[index / ELEMENT_BITS] & (1u << (index % ELEMENT_BITS)));
		else
			return false;
	}

	void BitArray::set(MemorySize index) {
		MemorySize offset = index / ELEMENT_BITS;
		taint(offset);
		bits[offset] |= 1u << (index % ELEMENT_BITS);
	}

	void BitArray::unset(MemorySize index) {
		MemorySize offset = index / ELEMENT_BITS;
		taint(offset);
		bits[offset] &= ~(1u << (index % ELEMENT_BITS));
	}

	void BitArray::set(MemorySize index, bool value) {
		MemorySize offset = index / ELEMENT_BITS;
		taint(offset);
		if(value)
			bits[offset] |= 1u << (index % ELEMENT_BITS);
		else
			bits[offset] &= ~(1u << (index % ELEMENT_BITS));
	}

	void BitArray::toggle(MemorySize index) {
		MemorySize offset = index / ELEMENT_BITS, rest = index % ELEMENT_BITS;
		taint(offset);
		unsigned mask = 1u << rest;
		if(bits[offset] & mask)
			bits[offset] &= ~mask;
		else
			bits[offset] |= mask;
	}

	int BitArray::compare(const BitArray& array) const {
		MemorySize compareSize = tainted < array.tainted ? tainted : array.tainted;
		if(compareSize) {
			int order = memcmp(bits, array.bits, compareSize * sizeof(unsigned));
			if(order)
				return order;
		}
		const unsigned *begin, *end;
		if(compareSize < size) {
			for(begin = bits + tainted, end = bits + size; begin != end; ++begin)
				if(*begin)
					return 1;
		}
		if(compareSize < array.size) {
			for(begin = array.bits + array.tainted, end = array.bits + array.size; begin != end; ++begin)
				if(*begin)
					return -1;
		}
		return (array.size < size) - (size < array.size);
	}

	bool BitArray::operator==(const BitArray& array) const {
		return !compare(array);
	}

	bool BitArray::operator<(const BitArray& array) const {
		return compare(array) < 0;
	}

	BitArray& BitArray::operator=(const BitArray& array) {
		MemorySize copySize = size < array.size ? size : array.size;
		tainted = copySize < array.tainted ? copySize : array.tainted;
		if(tainted)
			memcpy(bits, array.bits, tainted);
		return *this;
	}

	bool BitArray::isClear() const {
		const unsigned *begin = bits, *end = bits + tainted;
		for(; begin != end; ++begin)
			if(*begin)
				return false;
		return true;
	}

}}
