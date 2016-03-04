#include <cstring>

#include "ArbitraryPrecision.hpp"

using redengine::util::MemorySize;

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

	void ArbitraryPrecision::intAdd(const IntegerData&, const IntegerData&, bool&, unsigned*&, MemorySize&) {
		//TODO
	}

	void ArbitraryPrecision::intAdd(IntegerData& a, const IntegerData& b) {
		bool sign;
		unsigned* digits;
		MemorySize size;
		ArbitraryPrecision::intAdd(a, b, sign, digits, size);
		a.assign(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
	}

	void ArbitraryPrecision::intSub(const IntegerData&, const IntegerData&, bool&, unsigned*&, MemorySize&) {
		//TODO
	}

	void ArbitraryPrecision::intSub(IntegerData& a, const IntegerData& b) {
		bool sign;
		unsigned* digits;
		MemorySize size;
		ArbitraryPrecision::intSub(a, b, sign, digits, size);
		a.assign(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
	}

	void ArbitraryPrecision::intMul(const IntegerData&, const IntegerData&, bool&, unsigned*&, MemorySize&) {
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
