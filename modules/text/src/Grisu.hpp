#ifndef REDSTRAIN_MOD_TEXT_GRISU_HPP
#define REDSTRAIN_MOD_TEXT_GRISU_HPP

#include <string>
#include <stdint.h>
#include <redstrain/io/OutputStream.hpp>

#include "api.hpp"

namespace redengine {
namespace text {

	class REDSTRAIN_TEXT_API Grisu {

	  public:
		class REDSTRAIN_TEXT_API FakeFloat {

		  private:
			static const uint64_t MASK_32 = static_cast<uint64_t>(0xFFFFFFFFul);

		  public:
			uint64_t f;
			int32_t e;

		  private:
			FakeFloat(uint64_t, int32_t, bool);

		  public:
			FakeFloat(uint64_t, int32_t);
			FakeFloat(const FakeFloat&);

			void normalize();

			FakeFloat operator-(const FakeFloat&) const;
			FakeFloat operator*(const FakeFloat&) const;

			FakeFloat& operator-=(const FakeFloat&);
			FakeFloat& operator*=(const FakeFloat&);

		  public:
			static const FakeFloat ONE;
			static const FakeFloat TEN;
			static const FakeFloat ONE_TENTH;

		};

		class REDSTRAIN_TEXT_API DecimalRepresentation {

		  public:
			const uint32_t high, middle, low;
			const int32_t exponent;

		  public:
			DecimalRepresentation(uint32_t, uint32_t, uint32_t, int32_t);
			DecimalRepresentation(const DecimalRepresentation&);

		};

		typedef std::basic_string<unsigned char> DigitSequence;

	  public:
		static const int32_t K_MIN = static_cast<int32_t>(-289);
		static const int32_t K_MAX = static_cast<int32_t>(342);
		static const int32_t Q = static_cast<int32_t>(64);
		static const int32_t ALPHA = static_cast<int32_t>(0);
		static const int32_t ALPHA2 = static_cast<int32_t>(-59);

	  private:
		static const uint64_t CACHED_SIGNIFICANTS[];
		static const int32_t CACHED_EXPONENTS[];

	  private:
		static uint32_t computeCache(uint64_t*&, int32_t*&);
		static uint32_t computeCache(int32_t, int32_t, uint64_t*&, int32_t*&);
		static int32_t kComp(int32_t, int32_t);
		static FakeFloat cachedPower(int32_t);
		static DecimalRepresentation cutGrisu(const FakeFloat&, int32_t);
		static void digitGen(const FakeFloat&, FakeFloat, int32_t&, DigitSequence&);

	  public:
		static void generateCacheSource(io::OutputStream<char>&,
				const std::string&, const std::string&, const std::string&, const std::string&);
		static DecimalRepresentation grisu(const FakeFloat&);
		static int32_t grisu2(const FakeFloat&, DigitSequence&);

	};

}}

#endif /* REDSTRAIN_MOD_TEXT_GRISU_HPP */
