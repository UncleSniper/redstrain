#ifndef REDSTRAIN_MOD_MATH_BIGINTEGER_HPP
#define REDSTRAIN_MOD_MATH_BIGINTEGER_HPP

#include <cstring>
#include <redstrain/util/DeleteArray.hpp>
#include <redstrain/platform/MutexPoolLocker.hpp>

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
			return intData.sign;
		}

		int signum() const {
			return intData.signum();
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator=(const BigInteger<CopyPolicy>& other) {
			intData.assign(other.data());
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

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		int compare(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(intData, other.data());
		}

		BigInteger operator-() const {
			const unsigned* digits;
			util::MemorySize size;
			intData.compact(digits, size);
			if(!size)
				return BigInteger();
			return BigInteger(!intData.sign, digits, size);
		}

		void negate() {
			if(intData.signum())
				intData.sign = !intData.sign;
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
		void div(const BigInteger<CopyPolicy>& denominator, BigInteger& quotient, BigInteger& remainder) const {
			bool qsign, rsign;
			unsigned *qdigits, *rdigits;
			util::MemorySize qsize, rsize;
			ArbitraryPrecision::intDiv(intData, denominator.data(), qsign, qdigits, qsize, rsign, rdigits, rsize);
			quotient.intData.assign(qsign, qdigits, qsize, ArbitraryPrecision::INIT_MOVE);
			remainder.intData.assign(rsign, rdigits, rsize, ArbitraryPrecision::INIT_MOVE);
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

	template<>
	class BigInteger<ArbitraryPrecision::COPY_SHARED> {

	  private:
		ArbitraryPrecision::SharedIntegerData* intData;

	  private:
		void assignFromForeign(const ArbitraryPrecision::IntegerData& other) {
			if(intData->getReferenceCount() > 1u) {
				ArbitraryPrecision::SharedIntegerData* newData = new ArbitraryPrecision::SharedIntegerData(other);
				intData->unref();
				intData = newData;
			}
			else
				intData->assign(other);
		}

		typedef void (*SharedOperator)(
			const ArbitraryPrecision::IntegerData&,
			const ArbitraryPrecision::IntegerData&,
			bool&,
			unsigned*&,
			util::MemorySize&
		);

		typedef void (*PrivateOperator)(
			ArbitraryPrecision::IntegerData&,
			const ArbitraryPrecision::IntegerData&
		);

		void assignOp(const ArbitraryPrecision::IntegerData& other,
				SharedOperator sharedOp, PrivateOperator privateOp) {
			if(intData->getReferenceCount() > 1u) {
				bool sign;
				unsigned* digits;
				util::MemorySize size;
				sharedOp(*intData, other, sign, digits, size);
				util::DeleteArray<unsigned> deleteDigits(digits);
				ArbitraryPrecision::SharedIntegerData* newData = new ArbitraryPrecision::SharedIntegerData(sign,
						digits, size, ArbitraryPrecision::INIT_MOVE);
				deleteDigits.set();
				intData->unref();
				intData = newData;
			}
			else
				privateOp(*intData, other);
		}

	  public:
		BigInteger() : intData(new ArbitraryPrecision::SharedIntegerData) {}

		BigInteger(const ArbitraryPrecision::IntegerData& value)
				: intData(new ArbitraryPrecision::SharedIntegerData(value)) {}

		BigInteger(ArbitraryPrecision::IntegerData& value, ArbitraryPrecision::InitPolicy initPolicy)
				: intData(new ArbitraryPrecision::SharedIntegerData(value, initPolicy)) {}

		BigInteger(bool sign, const unsigned* digits, util::MemorySize size)
				: intData(new ArbitraryPrecision::SharedIntegerData(sign, digits, size)) {}

		BigInteger(bool sign, unsigned* digits, util::MemorySize size, ArbitraryPrecision::InitPolicy initPolicy)
				: intData(new ArbitraryPrecision::SharedIntegerData(sign, digits, size, initPolicy)) {}

		BigInteger(const BigInteger& value) : intData(value.intData) {
			intData->ref();
		}

		~BigInteger() {
			intData->unref();
		}

		inline const ArbitraryPrecision::IntegerData& data() const {
			return *intData;
		}

		inline bool isNegative() const {
			return intData->sign;
		}

		int signum() const {
			return intData->signum();
		}

		BigInteger& operator=(const BigInteger<ArbitraryPrecision::COPY_SHARED>& other) {
			other.intData->ref();
			intData->unref();
			intData = other.intData;
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator=(const BigInteger<CopyPolicy>& other) {
			assignFromForeign(other.data());
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator==(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) == 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator!=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) != 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator<(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) < 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator<=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) <= 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator>(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) > 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator>=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) >= 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		int compare(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data());
		}

		BigInteger operator-() const {
			const unsigned* digits;
			util::MemorySize size;
			intData->compact(digits, size);
			if(!size)
				return BigInteger(*this);
			return BigInteger(!intData->sign, digits, size);
		}

		void negate() {
			const unsigned* digits;
			util::MemorySize size;
			intData->compact(digits, size);
			if(!size)
				return;
			if(intData->getReferenceCount() > 1u) {
				ArbitraryPrecision::SharedIntegerData* newData
						= new ArbitraryPrecision::SharedIntegerData(!intData->sign, digits, size);
				intData->unref();
				intData = newData;
			}
			else
				intData->sign = !intData->sign;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator+(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intAdd(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator-(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intSub(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator*(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intMul(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator/(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intDiv(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator%(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intMod(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		void div(const BigInteger<CopyPolicy>& denominator, BigInteger& quotient, BigInteger& remainder) const {
			bool qsign, rsign;
			unsigned *qdigits, *rdigits;
			util::MemorySize qsize, rsize;
			ArbitraryPrecision::intDiv(*intData, denominator.data(), qsign, qdigits, qsize, rsign, rdigits, rsize);
			if(quotient.intData->getReferenceCount() > 1u) {
				util::DeleteArray<unsigned> deleteQuotient(qdigits);
				util::DeleteArray<unsigned> deleteRemainder(rdigits);
				ArbitraryPrecision::SharedIntegerData* qData = new ArbitraryPrecision::SharedIntegerData(qsign,
						qdigits, qsize, ArbitraryPrecision::INIT_MOVE);
				deleteQuotient.set();
				deleteRemainder.set();
				quotient.intData->unref();
				quotient.intData = qData;
			}
			else
				quotient.intData->assign(qsign, qdigits, qsize, ArbitraryPrecision::INIT_MOVE);
			if(remainder.intData->getReferenceCount() > 1u) {
				util::DeleteArray<unsigned> deleteRemainder(rdigits);
				ArbitraryPrecision::SharedIntegerData* rData = new ArbitraryPrecision::SharedIntegerData(rsign,
						rdigits, rsize, ArbitraryPrecision::INIT_MOVE);
				deleteRemainder.set();
				remainder.intData->unref();
				remainder.intData = rData;
			}
			else
				remainder.intData->assign(rsign, rdigits, rsize, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator+=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intAdd, ArbitraryPrecision::intAdd);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator-=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intSub, ArbitraryPrecision::intSub);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator*=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intMul, ArbitraryPrecision::intMul);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator/=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intDiv, ArbitraryPrecision::intDiv);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator%=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intMod, ArbitraryPrecision::intMod);
			return *this;
		}

	};

	template<>
	class BigInteger<ArbitraryPrecision::COPY_THREADED> {

	  private:
		ArbitraryPrecision::SharedIntegerData* intData;

	  private:
		void assignFromForeign(const ArbitraryPrecision::IntegerData& other) {
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> lock(*intData);
			if(intData->getReferenceCount() > 1u) {
				ArbitraryPrecision::SharedIntegerData* newData = new ArbitraryPrecision::SharedIntegerData(other);
				intData->unref();
				intData = newData;
			}
			else
				intData->assign(other);
			lock.release();
		}

		typedef void (*SharedOperator)(
			const ArbitraryPrecision::IntegerData&,
			const ArbitraryPrecision::IntegerData&,
			bool&,
			unsigned*&,
			util::MemorySize&
		);

		typedef void (*PrivateOperator)(
			ArbitraryPrecision::IntegerData&,
			const ArbitraryPrecision::IntegerData&
		);

		void assignOp(const ArbitraryPrecision::IntegerData& other,
				SharedOperator sharedOp, PrivateOperator privateOp) {
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> lock(*intData);
			if(intData->getReferenceCount() > 1u) {
				bool sign;
				unsigned* digits;
				util::MemorySize size;
				sharedOp(*intData, other, sign, digits, size);
				util::DeleteArray<unsigned> deleteDigits(digits);
				ArbitraryPrecision::SharedIntegerData* newData = new ArbitraryPrecision::SharedIntegerData(sign,
						digits, size, ArbitraryPrecision::INIT_MOVE);
				deleteDigits.set();
				intData->unref();
				intData = newData;
			}
			else
				privateOp(*intData, other);
			lock.release();
		}

	  public:
		BigInteger() : intData(new ArbitraryPrecision::SharedIntegerData) {}

		BigInteger(const ArbitraryPrecision::IntegerData& value)
				: intData(new ArbitraryPrecision::SharedIntegerData(value)) {}

		BigInteger(ArbitraryPrecision::IntegerData& value, ArbitraryPrecision::InitPolicy initPolicy)
				: intData(new ArbitraryPrecision::SharedIntegerData(value, initPolicy)) {}

		BigInteger(bool sign, const unsigned* digits, util::MemorySize size)
				: intData(new ArbitraryPrecision::SharedIntegerData(sign, digits, size)) {}

		BigInteger(bool sign, unsigned* digits, util::MemorySize size, ArbitraryPrecision::InitPolicy initPolicy)
				: intData(new ArbitraryPrecision::SharedIntegerData(sign, digits, size, initPolicy)) {}

		BigInteger(const BigInteger& value) : intData(value.intData) {
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> lock(*intData);
			intData->ref();
			lock.release();
		}

		~BigInteger() {
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> lock(*intData);
			intData->unref();
		}

		inline const ArbitraryPrecision::IntegerData& data() const {
			return *intData;
		}

		inline bool isNegative() const {
			return intData->sign;
		}

		int signum() const {
			return intData->signum();
		}

		BigInteger& operator=(const BigInteger<ArbitraryPrecision::COPY_THREADED>& other) {
			ArbitraryPrecision::SharedIntegerData* odata = other.intData;
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> lock(*intData);
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> olock(*odata);
			odata->ref();
			intData->unref();
			intData = odata;
			olock.release();
			lock.release();
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator=(const BigInteger<CopyPolicy>& other) {
			assignFromForeign(other.data());
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator==(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) == 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator!=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) != 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator<(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) < 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator<=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) <= 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator>(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) > 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		bool operator>=(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data()) >= 0;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		int compare(const BigInteger<CopyPolicy>& other) const {
			return ArbitraryPrecision::intCmp(*intData, other.data());
		}

		BigInteger operator-() const {
			const unsigned* digits;
			util::MemorySize size;
			intData->compact(digits, size);
			if(!size)
				return BigInteger(*this);
			return BigInteger(!intData->sign, digits, size);
		}

		void negate() {
			const unsigned* digits;
			util::MemorySize size;
			intData->compact(digits, size);
			if(!size)
				return;
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> lock(*intData);
			if(intData->getReferenceCount() > 1u) {
				ArbitraryPrecision::SharedIntegerData* newData
						= new ArbitraryPrecision::SharedIntegerData(!intData->sign, digits, size);
				intData->unref();
				intData = newData;
			}
			else
				intData->sign = !intData->sign;
			lock.release();
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator+(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intAdd(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator-(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intSub(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator*(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intMul(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator/(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intDiv(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger operator%(const BigInteger<CopyPolicy>& other) const {
			bool sign;
			unsigned* digits;
			util::MemorySize size;
			ArbitraryPrecision::intMod(*intData, other.data(), sign, digits, size);
			return BigInteger(sign, digits, size, ArbitraryPrecision::INIT_MOVE);
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		void div(const BigInteger<CopyPolicy>& denominator, BigInteger& quotient, BigInteger& remainder) const {
			bool qsign, rsign;
			unsigned *qdigits, *rdigits;
			util::MemorySize qsize, rsize;
			ArbitraryPrecision::intDiv(*intData, denominator.data(), qsign, qdigits, qsize, rsign, rdigits, rsize);
			util::DeleteArray<unsigned> deleteQuotient(qdigits);
			util::DeleteArray<unsigned> deleteRemainder(rdigits);
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> qlock(*quotient.intData);
			if(quotient.intData->getReferenceCount() > 1u) {
				ArbitraryPrecision::SharedIntegerData* qData = new ArbitraryPrecision::SharedIntegerData(qsign,
						qdigits, qsize, ArbitraryPrecision::INIT_MOVE);
				quotient.intData->unref();
				quotient.intData = qData;
			}
			else
				quotient.intData->assign(qsign, qdigits, qsize, ArbitraryPrecision::INIT_MOVE);
			deleteQuotient.set();
			qlock.release();
			platform::MutexPoolLocker<ArbitraryPrecision::IntegerData> rlock(*remainder.intData);
			if(remainder.intData->getReferenceCount() > 1u) {
				ArbitraryPrecision::SharedIntegerData* rData = new ArbitraryPrecision::SharedIntegerData(rsign,
						rdigits, rsize, ArbitraryPrecision::INIT_MOVE);
				remainder.intData->unref();
				remainder.intData = rData;
			}
			else
				remainder.intData->assign(rsign, rdigits, rsize, ArbitraryPrecision::INIT_MOVE);
			deleteRemainder.set();
			rlock.release();
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator+=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intAdd, ArbitraryPrecision::intAdd);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator-=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intSub, ArbitraryPrecision::intSub);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator*=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intMul, ArbitraryPrecision::intMul);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator/=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intDiv, ArbitraryPrecision::intDiv);
			return *this;
		}

		template<ArbitraryPrecision::CopyPolicy CopyPolicy>
		BigInteger& operator%=(const BigInteger<CopyPolicy>& other) {
			assignOp(other.data(), ArbitraryPrecision::intMod, ArbitraryPrecision::intMod);
			return *this;
		}

	};

}}

#endif /* REDSTRAIN_MOD_MATH_BIGINTEGER_HPP */
