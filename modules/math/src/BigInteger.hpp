#ifndef REDSTRAIN_MOD_MATH_BIGINTEGER_HPP
#define REDSTRAIN_MOD_MATH_BIGINTEGER_HPP

#include <cstring>

#include "ArbitraryPrecision.hpp"

namespace redengine {
namespace math {

	template<ArbitraryPrecision::CopyPolicy = ArbitraryPrecision::COPY_PRIVATE>
	class BigInteger;

	template<>
	class BigInteger<ArbitraryPrecision::COPY_PRIVATE> {

	  private:
		ArbitraryPrecision::IntegerData intData;

	  public:
		BigInteger() {}

		BigInteger(const ArbitraryPrecision::IntegerData& value) : intData(value) {}

		BigInteger(ArbitraryPrecision::IntegerData& value, ArbitraryPrecision::InitPolicy initPolicy)
				: intData(value, initPolicy) {}

		BigInteger(bool sign, const unsigned* digits, util::MemorySize size) : intData(sign, digits, size) {}

		BigInteger(bool sign, unsigned* digits, util::MemorySize size, ArbitraryPrecision::InitPolicy initPolicy)
				: intData(sign, digits, size, initPolicy) {}

		BigInteger(const BigInteger& value) : intData(value.intData) {}

		~BigInteger() {}

		inline const ArbitraryPrecision::IntegerData& data() const {
			return intData;
		}

		inline bool isNegative() const {
			return data.sign;
		}

		int sign() const {
			return data.sign();
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator=(const BigInteger<CopyPolicy>& other) {
			intData = other.data();
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator==(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(intData, other.data()) == 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator!=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(intData, other.data()) != 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator<(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(intData, other.data()) < 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator<=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(intData, other.data()) <= 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator>(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(intData, other.data()) > 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator>=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(intData, other.data()) >= 0;
		}

		BigInteger operator-() const {
			const unsigned* digits;
			util::MemorySize size;
			intData.compact(digits, size);
			if(!size)
				return BigInteger();
			return BigInteger(!intData.sign, digits, size);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator+(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intAdd(intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator-(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intSub(intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator*(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intMul(intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator/(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intDiv(intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator%(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intMod(intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator+=(const BigInteger<CopyPolicy>& other) {
			ArbitraryPrecision::intAdd(intData, other.data());
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator-=(const BigInteger<CopyPolicy>& other) {
			ArbitraryPrecision::intSub(intData, other.data());
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator*=(const BigInteger<CopyPolicy>& other) {
			ArbitraryPrecision::intMul(intData, other.data());
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator/=(const BigInteger<CopyPolicy>& other) {
			ArbitraryPrecision::intDiv(intData, other.data());
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator%=(const BigInteger<CopyPolicy>& other) {
			ArbitraryPrecision::intMod(intData, other.data());
			return *this;
		}

	};

}}

#endif /* REDSTRAIN_MOD_MATH_BIGINTEGER_HPP */
