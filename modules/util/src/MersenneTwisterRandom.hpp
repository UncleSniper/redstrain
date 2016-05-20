#ifndef REDSTRAIN_MOD_UTIL_MERSENNETWISTERRANDOM_HPP
#define REDSTRAIN_MOD_UTIL_MERSENNETWISTERRANDOM_HPP

#include "Random.hpp"

namespace redengine {
namespace util {

	class REDSTRAIN_UTIL_API MersenneTwisterRandom : public Random {

	  private:
		static const unsigned MT_W = 32u;
		static const unsigned MT_N = 624u;
		static const unsigned MT_M = 397u;
		static const unsigned MT_R = 31u;
		static const uint32_t MT_A = static_cast<uint32_t>(0x9908B0DFu);
		static const unsigned MT_U = 11u;
		static const uint32_t MT_D = static_cast<uint32_t>(0xFFFFFFFFu);
		static const unsigned MT_S = 7u;
		static const uint32_t MT_B = static_cast<uint32_t>(0x9D2C5680u);
		static const unsigned MT_T = 15u;
		static const uint32_t MT_C = static_cast<uint32_t>(0xEFC60000u);
		static const unsigned MT_L = 18u;
		static const uint32_t MT_F = static_cast<uint32_t>(0x6C078965u);

		static const uint32_t LOWER_MASK = (static_cast<uint32_t>(1u) << MersenneTwisterRandom::MT_R)
				- static_cast<uint32_t>(1u);
		static const uint32_t UPPER_MASK = ~MersenneTwisterRandom::LOWER_MASK;

		static const uint32_t DEFAULT_SEED = static_cast<uint32_t>(0xDEADBEEFu);

	  private:
		uint32_t mt[MersenneTwisterRandom::MT_N];
		unsigned index;

	  private:
		void seedMT(uint32_t);
		uint32_t extract();
		void twist();

	  public:
		MersenneTwisterRandom();
		MersenneTwisterRandom(const MersenneTwisterRandom&);

		virtual MemorySize getSeedSize() const;
		virtual void seed(const char*);
		virtual void seedFrom(Random&);
		virtual void randomBytes(char*, MemorySize);

	};

}}

#endif /* REDSTRAIN_MOD_UTIL_MERSENNETWISTERRANDOM_HPP */
