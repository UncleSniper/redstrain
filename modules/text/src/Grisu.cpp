#include <cmath>
#include <redstrain/util/IntegerLog.hpp>
#include <redstrain/util/DeleteArray.hpp>
#include <redstrain/util/StringUtils.hpp>
#include <redstrain/io/streamtypes.hpp>
#include <redstrain/io/streamoperators.hpp>

#include "Grisu.hpp"
#include "GrisuError.hpp"

using std::string;
using redengine::util::Appender;
using redengine::io::OutputStream;
using redengine::util::IntegerLog;
using redengine::util::DeleteArray;
using redengine::util::StringUtils;
using redengine::util::IntegerBounds;
using redengine::io::DefaultConfiguredOutputStream;
using redengine::io::endln;
using redengine::io::shift;
using redengine::io::indent;
using redengine::io::unshift;
using redengine::io::DECIMAL;
using redengine::io::operator<<;
using redengine::io::HEXADECIMAL;
using redengine::io::config::base;

namespace redengine {
namespace text {

	// ======== FakeFloat ========

	const Grisu::FakeFloat Grisu::FakeFloat::ONE(static_cast<uint64_t>(1u), static_cast<int32_t>(0), true);
	const Grisu::FakeFloat Grisu::FakeFloat::TEN(static_cast<uint64_t>(10u), static_cast<int32_t>(0), true);
	const Grisu::FakeFloat Grisu::FakeFloat::ONE_TENTH((static_cast<uint64_t>(0xCCCCCCCCul) << 32)
			| static_cast<uint64_t>(0xCCCCCCCCul), static_cast<int32_t>(-67), true);

	Grisu::FakeFloat::FakeFloat(uint64_t f, int32_t e, bool n) : f(f), e(e) {
		if(n)
			normalize();
	}

	Grisu::FakeFloat::FakeFloat(uint64_t f, int32_t e) : f(f), e(e) {}

	Grisu::FakeFloat::FakeFloat(const FakeFloat& value) : f(value.f), e(value.e) {}

	void Grisu::FakeFloat::normalize() {
		while(!((f >> 63) & static_cast<uint64_t>(1u))) {
			f <<= 1;
			--e;
		}
	}

	Grisu::FakeFloat Grisu::FakeFloat::operator-(const FakeFloat& other) const {
		if(e != other.e)
			throw GrisuError("FakeFloat subtraction with different exponents: x.e = "
					+ StringUtils::toString(e) + ", y.e = " + StringUtils::toString(other.e));
		if(f < other.f)
			throw GrisuError("FakeFloat subtraction would yield negative value: x.f = "
					+ StringUtils::toString(f) + ", y.f = " + StringUtils::toString(other.f));
		return FakeFloat(f - other.f, e);
	}

	Grisu::FakeFloat Grisu::FakeFloat::operator*(const FakeFloat& other) const {
		uint64_t a = f >> 32, b = f & FakeFloat::MASK_32;
		uint64_t c = other.f >> 32, d = other.f & FakeFloat::MASK_32;
		uint64_t ac = a * c, bc = b * c, ad = a * d, bd = b * d;
		uint64_t tmp = (bd >> 32) + (ad & FakeFloat::MASK_32) + (bc & FakeFloat::MASK_32);
		tmp += static_cast<uint64_t>(1u) << 31;
		return FakeFloat(
			ac + (ad >> 32) + (bc >> 32) + (tmp >> 32),
			e + other.e + static_cast<int32_t>(64)
		);
	}

	Grisu::FakeFloat& Grisu::FakeFloat::operator-=(const FakeFloat& other) {
		if(e != other.e)
			throw GrisuError("FakeFloat subtraction with different exponents: x.e = "
					+ StringUtils::toString(e) + ", y.e = " + StringUtils::toString(other.e));
		if(f < other.f)
			throw GrisuError("FakeFloat subtraction would yield negative value: x.f = "
					+ StringUtils::toString(f) + ", y.f = " + StringUtils::toString(other.f));
		f -= other.f;
		return *this;
	}

	Grisu::FakeFloat& Grisu::FakeFloat::operator*=(const FakeFloat& other) {
		uint64_t a = f >> 32, b = f & FakeFloat::MASK_32;
		uint64_t c = other.f >> 32, d = other.f & FakeFloat::MASK_32;
		uint64_t ac = a * c, bc = b * c, ad = a * d, bd = b * d;
		uint64_t tmp = (bd >> 32) + (ad & FakeFloat::MASK_32) + (bc & FakeFloat::MASK_32);
		tmp += static_cast<uint64_t>(1u) << 31;
		f = ac + (ad >> 32) + (bc >> 32) + (tmp >> 32);
		e += other.e + static_cast<int32_t>(64);
		return *this;
	}

	// ======== DecimalRepresentation ========

	Grisu::DecimalRepresentation::DecimalRepresentation(uint32_t high, uint32_t middle, uint32_t low,
			int32_t exponent) : high(high), middle(middle), low(low), exponent(exponent) {}

	Grisu::DecimalRepresentation::DecimalRepresentation(const DecimalRepresentation& representation)
			: high(representation.high), middle(representation.middle), low(representation.low),
			exponent(representation.exponent) {}

	// ======== Grisu ========

	uint32_t Grisu::computeCache(uint64_t*& significants, int32_t*& exponents) {
		return Grisu::computeCache(Grisu::K_MIN, Grisu::K_MAX, significants, exponents);
	}

	uint32_t Grisu::computeCache(int32_t kMin, int32_t kMax, uint64_t*& significants, int32_t*& exponents) {
		if(kMin >= kMax)
			throw GrisuError("Minimal exponent must be less than maximal exponent: "
					+ StringUtils::toString(kMin) + " >= " + StringUtils::toString(kMax));
		uint32_t count = static_cast<uint32_t>(kMax - kMin) + static_cast<uint32_t>(1u);
		DeleteArray<uint64_t> sigs(new uint64_t[count]);
		DeleteArray<int32_t> exps(new int32_t[count]);
		// zero
		FakeFloat v(FakeFloat::ONE);
		unsigned index;
		if(kMin <= static_cast<int32_t>(0) && kMax >= static_cast<int32_t>(0)) {
			index = static_cast<unsigned>(-kMin);
			sigs[index] = v.f;
			exps[index] = v.e;
		}
		// positive
		int32_t current;
		for(current = static_cast<int32_t>(1); current <= kMax; ++current) {
			v *= FakeFloat::TEN;
			v.normalize();
			if(current >= kMin) {
				index = static_cast<unsigned>(current - kMin);
				sigs[index] = v.f;
				exps[index] = v.e;
			}
		}
		// negative
		v = FakeFloat::ONE;
		for(current = static_cast<int32_t>(-1); current >= kMin; --current) {
			v *= FakeFloat::ONE_TENTH;
			v.normalize();
			if(current <= kMax) {
				index = static_cast<unsigned>(current - kMin);
				sigs[index] = v.f;
				exps[index] = v.e;
			}
		}
		significants = sigs.set();
		exponents = exps.set();
		return count;
	}

	struct GrisuCacheGenNamespaceSink : Appender<string> {

		unsigned level;
		DefaultConfiguredOutputStream<char>::Stream& output;

		GrisuCacheGenNamespaceSink(DefaultConfiguredOutputStream<char>::Stream& output)
				: level(0u), output(output) {}

		void closeNamespaces();

		virtual void append(const string&);
		virtual void doneAppending();

	};

	void GrisuCacheGenNamespaceSink::append(const string& segment) {
		string seg(StringUtils::trim(segment));
		if(seg.empty())
			return;
		output << indent << "namespace " << seg << " {" << endln;
		++level;
	}

	void GrisuCacheGenNamespaceSink::doneAppending() {
		if(level)
			output << endln << shift;
	}

	void GrisuCacheGenNamespaceSink::closeNamespaces() {
		if(!level)
			return;
		output << unshift << endln << indent;
		do
			output << '}';
		while(--level);
		output << endln;
	}

	void Grisu::generateCacheSource(OutputStream<char>& output, const string& ns, const string& className,
			const string& significantsName, const string& exponentsName) {
		DeleteArray<uint64_t> significants;
		DeleteArray<int32_t> exponents;
		uint32_t count = Grisu::computeCache(significants.pointerLocation(), exponents.pointerLocation());
		DefaultConfiguredOutputStream<char>::Stream formatted(output);
		formatted << base<char>(HEXADECIMAL);
		formatted << "#include <stdint.h>" << endln << endln;
		if(!className.empty())
			formatted << "#include \"" << className << ".hpp\"" << endln << endln;
		GrisuCacheGenNamespaceSink nsSink(formatted);
		StringUtils::split(ns, "::", nsSink);
		formatted << indent;
		if(className.empty())
			formatted << "extern ";
		formatted << "const uint64_t ";
		if(!className.empty())
			formatted << className << "::";
		if(significantsName.empty())
			formatted << "SIGNIFICANTS";
		else
			formatted << significantsName;
		formatted << "[] = {" << shift << endln;
		uint32_t index;
		for(index = static_cast<uint32_t>(0u); index < count; ++index) {
			formatted << indent << "(static_cast<uint64_t>(0x" << (significants[index] >> 32) << "ul) << 32)";
			formatted << " | static_cast<uint64_t>(0x"
					<< (significants[index] & static_cast<uint64_t>(0xFFFFFFFFul)) << "ul)";
			if(index < count - static_cast<uint32_t>(1u))
				formatted << ',';
			formatted << endln;
		}
		formatted << unshift << indent << "};" << endln << endln;
		formatted << base<char>(DECIMAL);
		formatted << indent;
		if(className.empty())
			formatted << "extern ";
		formatted << "const int32_t ";
		if(!className.empty())
			formatted << className << "::";
		if(exponentsName.empty())
			formatted << "EXPONENTS";
		else
			formatted << exponentsName;
		formatted << "[] = {" << shift << endln;
		for(index = static_cast<uint32_t>(0u); index < count; ++index) {
			formatted << indent << "static_cast<int32_t>(" << exponents[index] << ')';
			if(index < count - static_cast<uint32_t>(1u))
				formatted << ',';
			formatted << endln;
		}
		formatted << unshift << indent << "};" << endln;
		nsSink.closeNamespaces();
	}

	static const double M_1_LD_10 = 1.0 / log2(10.0);

	int32_t Grisu::kComp(int32_t e, int32_t alpha) {
		return static_cast<int32_t>(ceil(static_cast<double>(alpha - e
				+ Grisu::Q - static_cast<int32_t>(1)) * M_1_LD_10));
	}

	Grisu::FakeFloat Grisu::cachedPower(int32_t k) {
		if(k < Grisu::K_MIN || k > Grisu::K_MAX)
			throw GrisuError("Exponent for cached Grisu power out of range: " + StringUtils::toString(k));
		unsigned index = static_cast<unsigned>(k - Grisu::K_MIN);
		return FakeFloat(Grisu::CACHED_SIGNIFICANTS[index], Grisu::CACHED_EXPONENTS[index]);
	}

	Grisu::DecimalRepresentation Grisu::grisu(const FakeFloat& value) {
		FakeFloat w(value);
		w.normalize();
		int32_t mk = Grisu::kComp(w.e + Grisu::Q, Grisu::ALPHA);
		FakeFloat c_mk(Grisu::cachedPower(mk));
		return Grisu::cutGrisu(w * c_mk, mk);
	}

	static const uint64_t TEN7 = static_cast<uint64_t>(10000000ul);

	Grisu::DecimalRepresentation Grisu::cutGrisu(const FakeFloat& d, int32_t mk) {
		uint32_t low = static_cast<uint32_t>((d.f % (TEN7 >> d.e)) << d.e);
		uint64_t tmp = d.f / (TEN7 >> d.e);
		uint32_t middle = static_cast<uint32_t>(tmp % TEN7);
		uint32_t high = static_cast<uint32_t>(tmp / TEN7);
		return DecimalRepresentation(high, middle, low, -mk);
	}

	int32_t Grisu::grisu2(const FakeFloat& value, DigitSequence& sink) {
		// Since the idea is that we grok double precision floats,
		// we have plenty of bits left after putting the 52+1 bit
		// mantissa into a uint64_t, so we can easily invest one
		// of them to add the extra binary place we need to get
		// the mean between neighbors.
		FakeFloat wPlus(value.f, value.e - static_cast<int32_t>(1));
		FakeFloat wMinus(wPlus);
		wPlus.f = (wPlus.f << 1) + static_cast<uint64_t>(1u);
		wMinus.f = (wMinus.f << 1) - static_cast<uint64_t>(1u);
		int32_t originalE = wPlus.e;
		wPlus.normalize();
		int32_t deltaE = originalE - wPlus.e;
		wMinus.f <<= deltaE;
		wMinus.e -= deltaE;
		int32_t mk = Grisu::kComp(wPlus.e + Grisu::Q, Grisu::ALPHA2);
		FakeFloat c_mk(Grisu::cachedPower(mk));
		FakeFloat mMinus(wMinus * c_mk), mPlus(wPlus * c_mk);
		++mMinus.f;
		--mPlus.f;
		mk = -mk;
		Grisu::digitGen(mPlus, mPlus - mMinus, mk, sink);
		return mk;
	}

	static const uint32_t TEN9_32 = static_cast<uint32_t>(1000000000u);
	static const uint64_t TEN1 = static_cast<uint64_t>(10ul);
	static const uint32_t TEN1_32 = static_cast<uint32_t>(10u);

	void Grisu::digitGen(const FakeFloat& mPlus, FakeFloat delta, int32_t& k, DigitSequence& sink) {
		sink.clear();
		sink.reserve(static_cast<DigitSequence::size_type>(IntegerLog<
			uint64_t,
			static_cast<uint64_t>(10u),
			IntegerBounds<uint64_t>::MAX
		>::EXPONENT) * static_cast<DigitSequence::size_type>(2u));
		FakeFloat one(static_cast<uint64_t>(1u) << -mPlus.e, mPlus.e);
		uint32_t p1 = static_cast<uint32_t>(mPlus.f >> -one.e);
		uint64_t p2 = mPlus.f & (one.f - static_cast<uint64_t>(1u));
		int32_t kappa = static_cast<int32_t>(10);
		uint32_t div = TEN9_32, d;
		unsigned len = 0u;
		while(kappa > static_cast<int32_t>(0)) {
			d = p1 / div;
			if(d || len) {
				sink += static_cast<unsigned char>(d);
				++len;
			}
			p1 %= div;
			--kappa;
			div /= TEN1_32;
			if((static_cast<uint64_t>(p1) << -one.e) + p2 <= delta.f) {
				k += kappa;
				return;
			}
		}
		do {
			p2 *= TEN1;
			d = static_cast<uint32_t>(p2 >> -one.e);
			if(d || len) {
				sink += static_cast<unsigned char>(d);
				++len;
			}
			p2 &= one.f - static_cast<uint64_t>(1u);
			--kappa;
			delta.f *= TEN1;
		} while(p2 > delta.f);
		k += kappa;
	}

}}
