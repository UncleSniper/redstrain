#include <cstring>
#include <redstrain/util/IntegerSplit.hpp>

#include "ArbitraryPrecision.hpp"

using redengine::util::MemorySize;
using redengine::util::IntegerSplit;

namespace redengine {
namespace math {

	// ======== IntegerData ========

	ArbitraryPrecision::IntegerData::IntegerData()
			: sign(false), digits(NULL), size(static_cast<MemorySize>(0u)) {}

	ArbitraryPrecision::IntegerData::IntegerData(bool sign, const unsigned* digits, MemorySize size)
			: sign(sign), digits(size ? new unsigned[size] : NULL), size(size) {
		if(size)
			memcpy(this->digits, digits, static_cast<size_t>(size) * sizeof(unsigned));
	}

	ArbitraryPrecision::IntegerData::IntegerData(bool sign, unsigned* digits, MemorySize size,
			InitPolicy initPolicy) : sign(sign), size(size) {
		if(!size)
			digits = NULL;
		else if(initPolicy == INIT_COPY) {
			this->digits = new unsigned[size];
			memcpy(this->digits, digits, static_cast<size_t>(size) * sizeof(unsigned));
		}
		else
			this->digits = digits;
	}

	ArbitraryPrecision::IntegerData::IntegerData(IntegerData& data, InitPolicy initPolicy)
			: sign(data.sign), size(data.size) {
		if(!size)
			digits = NULL;
		else if(initPolicy == INIT_COPY) {
			digits = new unsigned[size];
			memcpy(digits, data.digits, static_cast<size_t>(size) * sizeof(unsigned));
		}
		else {
			digits = data.digits;
			data.sign = false;
			data.digits = NULL;
			data.size = static_cast<MemorySize>(0u);
		}
	}

	ArbitraryPrecision::IntegerData::IntegerData(const IntegerData& data)
			: sign(data.sign), digits(data.size ? new unsigned[data.size] : NULL), size(data.size) {
		if(size)
			memcpy(digits, data.digits, static_cast<size_t>(size) * sizeof(unsigned));
	}

	ArbitraryPrecision::IntegerData::~IntegerData() {
		if(size)
			delete[] digits;
	}

	ArbitraryPrecision::IntegerData& ArbitraryPrecision::IntegerData::operator=(const IntegerData& other) {
		assign(other);
		return *this;
	}

	void ArbitraryPrecision::IntegerData::assign(const IntegerData& other) {
		if(this == &other)
			return;
		unsigned* newDigits;
		if(other.size) {
			newDigits = new unsigned[other.size];
			memcpy(newDigits, other.digits, static_cast<size_t>(other.size) * sizeof(unsigned));
		}
		else
			newDigits = NULL;
		if(size)
			delete[] digits;
		sign = other.sign;
		digits = newDigits;
		size = other.size;
	}

	void ArbitraryPrecision::IntegerData::assign(IntegerData& other, InitPolicy initPolicy) {
		if(this == &other) {
			if(initPolicy != INIT_COPY)
				other.clear();
			return;
		}
		unsigned* newDigits;
		if(!other.size)
			newDigits = NULL;
		else if(initPolicy == INIT_COPY) {
			newDigits = new unsigned[other.size];
			memcpy(newDigits, other.digits, static_cast<size_t>(other.size) * sizeof(unsigned));
		}
		else
			newDigits = other.digits;
		if(size)
			delete[] digits;
		sign = other.sign;
		digits = newDigits;
		size = other.size;
		if(size && initPolicy != INIT_COPY) {
			other.sign = false;
			other.digits = NULL;
			other.size = static_cast<MemorySize>(0u);
		}
	}

	void ArbitraryPrecision::IntegerData::assign(bool sign, const unsigned* digits, MemorySize size) {
		unsigned* newDigits;
		if(size) {
			newDigits = new unsigned[size];
			memcpy(newDigits, digits, static_cast<size_t>(size) * sizeof(unsigned));
		}
		else
			newDigits = NULL;
		if(this->size)
			delete[] this->digits;
		this->sign = sign;
		this->digits = newDigits;
		this->size = size;
	}

	void ArbitraryPrecision::IntegerData::assign(bool sign, unsigned* digits, MemorySize size,
			InitPolicy initPolicy) {
		unsigned* newDigits;
		if(!size)
			newDigits = NULL;
		else if(initPolicy == INIT_COPY) {
			newDigits = new unsigned[size];
			memcpy(newDigits, digits, static_cast<size_t>(size) * sizeof(unsigned));
		}
		else
			newDigits = digits;
		if(this->size)
			delete[] this->digits;
		this->sign = sign;
		this->digits = newDigits;
		this->size = size;
	}

	void ArbitraryPrecision::IntegerData::clear() {
		if(!size)
			return;
		delete[] digits;
		sign = false;
		digits = NULL;
		size = static_cast<MemorySize>(0u);
	}

	MemorySize ArbitraryPrecision::IntegerData::usedSize() const {
		MemorySize usz = size;
		const unsigned* d = digits;
		while(usz && !*d) {
			--usz;
			++d;
		}
		return usz;
	}

	void ArbitraryPrecision::IntegerData::compact(const unsigned*& digits, MemorySize& size) const {
		MemorySize usz = this->size;
		const unsigned* d = this->digits;
		while(usz && !*d) {
			--usz;
			++d;
		}
		digits = d;
		size = usz;
	}

	int ArbitraryPrecision::IntegerData::signum() const {
		if(sign)
			return -1;
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < size; ++u) {
			if(digits[u])
				return 1;
		}
		return 0;
	}

	MemorySize ArbitraryPrecision::IntegerData::bitCount(const unsigned* digits, MemorySize size) {
		if(!size)
			return size;
		return size * static_cast<MemorySize>(IntegerSplit<unsigned>::WIDTH)
				- static_cast<MemorySize>(IntegerSplit<unsigned>::countLeadingZeroes(*digits));
	}

	// ======== SharedIntegerData ========

	ArbitraryPrecision::SharedIntegerData::SharedIntegerData() {}

	ArbitraryPrecision::SharedIntegerData::SharedIntegerData(bool sign, const unsigned* digits, MemorySize size)
			: IntegerData(sign, digits, size) {}

	ArbitraryPrecision::SharedIntegerData::SharedIntegerData(bool sign, unsigned* digits, MemorySize size,
			InitPolicy initPolicy) : IntegerData(sign, digits, size, initPolicy) {}

	ArbitraryPrecision::SharedIntegerData::SharedIntegerData(IntegerData& data, InitPolicy initPolicy)
			: IntegerData(data, initPolicy) {}

	ArbitraryPrecision::SharedIntegerData::SharedIntegerData(const IntegerData& data) : IntegerData(data) {}

	ArbitraryPrecision::SharedIntegerData::SharedIntegerData(const SharedIntegerData& data)
			: IntegerData(data), ReferenceCounted(data) {}

	ArbitraryPrecision::SharedIntegerData::~SharedIntegerData() {}

	// ======== ArbitraryPrecision ========

	int ArbitraryPrecision::intCmp(const IntegerData& a, const IntegerData& b) {
		if(a.sign != b.sign)
			return a.sign ? -1 : 1;
		const unsigned *aDigits = a.digits, *bDigits = b.digits;
		MemorySize commonSize;
		if(a.size < b.size) {
			commonSize = a.size;
			MemorySize prefix = b.size - a.size;
			do {
				if(*bDigits)
					return a.sign ? 1 : -1;
				++bDigits;
			} while(--prefix);
		}
		else if(a.size > b.size) {
			commonSize = b.size;
			MemorySize prefix = a.size - b.size;
			do {
				if(*aDigits)
					return a.sign ? -1 : 1;
				++aDigits;
			} while(--prefix);
		}
		else
			commonSize = a.size;
		while(commonSize) {
			if(*aDigits < *bDigits)
				return a.sign ? 1 : -1;
			if(*aDigits > *bDigits)
				return a.sign ? -1 : 1;
			++aDigits;
			++bDigits;
			--commonSize;
		}
		return 0;
	}

	static inline MemorySize digitsNeededForBits(MemorySize bits) {
		return (bits + static_cast<MemorySize>(IntegerSplit<unsigned>::WIDTH - 1u))
				/ static_cast<MemorySize>(IntegerSplit<unsigned>::WIDTH);
	}

	static void intAddImpl(const ArbitraryPrecision::IntegerData& a, const ArbitraryPrecision::IntegerData& b,
			unsigned*& resultDigits, MemorySize& resultSize) {
		const unsigned *aDigits, *bDigits;
		MemorySize aSize, bSize;
		a.compact(aDigits, aSize);
		b.compact(bDigits, bSize);
		if(!aSize) {
			if(!bSize) {
				resultDigits = NULL;
				resultSize = static_cast<MemorySize>(0u);
			}
			else {
				resultDigits = new unsigned[bSize];
				memcpy(resultDigits, bDigits, static_cast<size_t>(bSize) * sizeof(unsigned));
				resultSize = bSize;
			}
			return;
		}
		if(!bSize) {
			resultDigits = new unsigned[aSize];
			memcpy(resultDigits, aDigits, static_cast<size_t>(aSize) * sizeof(unsigned));
			resultSize = aSize;
			return;
		}
		MemorySize aBits = ArbitraryPrecision::IntegerData::bitCount(aDigits, aSize);
		MemorySize bBits = ArbitraryPrecision::IntegerData::bitCount(bDigits, bSize);
		MemorySize commonSize, restSize;
		if(aBits > bBits) {
			resultSize = digitsNeededForBits(aBits + static_cast<MemorySize>(1u));
			commonSize = bSize;
			restSize = aSize - bSize;
		}
		else {
			resultSize = digitsNeededForBits(bBits + static_cast<MemorySize>(1u));
			commonSize = aSize;
			restSize = bSize - aSize;
		}
		unsigned* digits = (resultDigits = new unsigned[resultSize]) + resultSize;
		aDigits += aSize;
		bDigits += bSize;
		bool carry = false, nextCarry;
		for(; commonSize; --commonSize) {
			*--digits = *--aDigits + *--bDigits;
			nextCarry = *digits < *aDigits;
			if(carry) {
				if(!++*digits)
					nextCarry = true;
			}
			carry = nextCarry;
		}
		const unsigned* restDigits = aSize > bSize ? aDigits : bDigits;
		for(; carry && restSize; --restSize)
			carry = !(*--digits = *--restDigits + 1u);
		for(; restSize; --restSize)
			*--digits = *--restDigits;
		if(digits != resultDigits)
			*resultDigits = static_cast<unsigned>(carry);
	}

	static void intSubOrdered(const unsigned* aDigits, MemorySize aSize, const unsigned* bDigits, MemorySize bSize,
			unsigned*& resultDigits, MemorySize& resultSize) {
		// note that aSize && bSize && aSize >= bSize and, generally, a > b
		resultSize = aSize;
		resultDigits = new unsigned[aSize];
		const unsigned *a = aDigits + aSize, *b = bDigits + bSize;
		unsigned* r = resultDigits + aSize;
		MemorySize commonSize = bSize;
		bool carryIn, carryOut = false;
		do {
			--a;
			unsigned bd = *--b;
			carryIn = (carryOut && !++bd) || bd > *a;
			*--r = carryIn ? -(bd - *a) : *a - bd;
			carryOut = carryIn;
		} while(--commonSize);
		MemorySize restSize = aSize - bSize;
		for(; carryOut && restSize; --restSize)
			*--r = (carryOut = !*--a) ? ~0u : *a - 1u;
		for(; restSize; --restSize)
			*--r = *--a;
	}

	static void intSubImpl(const ArbitraryPrecision::IntegerData& a, const ArbitraryPrecision::IntegerData& b,
			bool& resultSign, unsigned*& resultDigits, MemorySize& resultSize) {
		const unsigned *aDigits, *bDigits;
		MemorySize aSize, bSize;
		a.compact(aDigits, aSize);
		b.compact(bDigits, bSize);
		if(!aSize) {
			if(!bSize) {
				resultSign = false;
				resultDigits = NULL;
				resultSize = static_cast<MemorySize>(0u);
			}
			else {
				resultSign = true;
				resultDigits = new unsigned[bSize];
				memcpy(resultDigits, bDigits, static_cast<size_t>(bSize) * sizeof(unsigned));
				resultSize = bSize;
			}
			return;
		}
		if(!bSize) {
			resultSign = false;
			resultDigits = new unsigned[aSize];
			memcpy(resultDigits, aDigits, static_cast<size_t>(aSize) * sizeof(unsigned));
			resultSize = aSize;
			return;
		}
		MemorySize aBits = ArbitraryPrecision::IntegerData::bitCount(aDigits, aSize);
		MemorySize bBits = ArbitraryPrecision::IntegerData::bitCount(bDigits, bSize);
		if(aBits > bBits) {
			intSubOrdered(aDigits, aSize, bDigits, bSize, resultDigits, resultSize);
			resultSign = false;
		}
		else if(bBits > aBits) {
			intSubOrdered(bDigits, bSize, aDigits, aSize, resultDigits, resultSize);
			resultSign = true;
		}
		else {
			for(; aSize; --aSize) {
				if(*aDigits > *bDigits) {
					intSubOrdered(aDigits, aSize, bDigits, aSize, resultDigits, resultSize);
					resultSign = false;
					return;
				}
				if(*bDigits > *aDigits) {
					intSubOrdered(bDigits, aSize, aDigits, aSize, resultDigits, resultSize);
					resultSign = true;
					return;
				}
				++aDigits;
				++bDigits;
			}
			resultSign = false;
			resultDigits = NULL;
			resultSize = static_cast<MemorySize>(0u);
		}
	}

	void ArbitraryPrecision::intAdd(const IntegerData& a, const IntegerData& b,
			bool& resultSign, unsigned*& resultDigits, MemorySize& resultSize) {
		if(a.sign) {
			if(b.sign) {
				intAddImpl(a, b, resultDigits, resultSize);
				resultSign = true;
			}
			else
				intSubImpl(b, a, resultSign, resultDigits, resultSize);
		}
		else {
			if(b.sign)
				intSubImpl(a, b, resultSign, resultDigits, resultSize);
			else {
				intAddImpl(a, b, resultDigits, resultSize);
				resultSign = false;
			}
		}
	}

	void ArbitraryPrecision::intAdd(IntegerData& a, const IntegerData& b) {
		bool sign;
		unsigned* digits;
		MemorySize size;
		ArbitraryPrecision::intAdd(a, b, sign, digits, size);
		a.assign(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
	}

	void ArbitraryPrecision::intSub(const IntegerData& a, const IntegerData& b,
			bool& resultSign, unsigned*& resultDigits, MemorySize& resultSize) {
		if(a.sign) {
			if(b.sign)
				intSubImpl(b, a, resultSign, resultDigits, resultSize);
			else {
				intAddImpl(a, b, resultDigits, resultSize);
				resultSign = true;
			}
		}
		else {
			if(b.sign) {
				intAddImpl(a, b, resultDigits, resultSize);
				resultSign = false;
			}
			else
				intSubImpl(a, b, resultSign, resultDigits, resultSize);
		}
	}

	void ArbitraryPrecision::intSub(IntegerData& a, const IntegerData& b) {
		bool sign;
		unsigned* digits;
		MemorySize size;
		ArbitraryPrecision::intSub(a, b, sign, digits, size);
		a.assign(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
	}

	void ArbitraryPrecision::intMul(const IntegerData& a, const IntegerData& b,
			bool& resultSign, unsigned*& resultDigits, MemorySize& resultSize) {
		const unsigned *aDigits, *bDigits;
		MemorySize aSize, bSize;
		a.compact(aDigits, aSize);
		b.compact(bDigits, bSize);
		if(!aSize || !bSize) {
			resultSign = false;
			resultDigits = NULL;
			resultSize = static_cast<MemorySize>(0u);
			return;
		}
		resultSign = a.sign != b.sign;
		resultSize = aSize + bSize;
		resultDigits = new unsigned[resultSize];
		//TODO
	}

	void ArbitraryPrecision::intMul(IntegerData& a, const IntegerData& b) {
		bool sign;
		unsigned* digits;
		MemorySize size;
		ArbitraryPrecision::intMul(a, b, sign, digits, size);
		a.assign(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
	}

	void ArbitraryPrecision::intDiv(const IntegerData&, const IntegerData&, bool&, unsigned*&, MemorySize&) {
		//TODO
	}

	void ArbitraryPrecision::intDiv(IntegerData& a, const IntegerData& b) {
		bool sign;
		unsigned* digits;
		MemorySize size;
		ArbitraryPrecision::intDiv(a, b, sign, digits, size);
		a.assign(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
	}

	void ArbitraryPrecision::intMod(const IntegerData&, const IntegerData&, bool&, unsigned*&, MemorySize&) {
		//TODO
	}

	void ArbitraryPrecision::intMod(IntegerData& a, const IntegerData& b) {
		bool sign;
		unsigned* digits;
		MemorySize size;
		ArbitraryPrecision::intMod(a, b, sign, digits, size);
		a.assign(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
	}

	void ArbitraryPrecision::intDiv(const IntegerData&, const IntegerData&, bool&, unsigned*&, MemorySize&, bool&, unsigned*&, MemorySize&) {
		//TODO
	}

}}
