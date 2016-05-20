#include <cstring>

#include "MersenneTwisterRandom.hpp"

namespace redengine {
namespace util {

	MersenneTwisterRandom::MersenneTwisterRandom() {}

	MersenneTwisterRandom::MersenneTwisterRandom(const MersenneTwisterRandom& random) : Random(random) {}

	MemorySize MersenneTwisterRandom::getSeedSize() const {
		return static_cast<MemorySize>(4u);
	}

	void MersenneTwisterRandom::seedMT(uint32_t seed) {
		index = MersenneTwisterRandom::MT_N;
		*mt = seed;
		unsigned u;
		for(u = 1u; u < MersenneTwisterRandom::MT_N; ++u) {
			uint32_t prev = mt[u - 1u];
			mt[u] = MersenneTwisterRandom::MT_F * (prev ^ (prev >> (MersenneTwisterRandom::MT_W - 2u)));
		}
	}

	void MersenneTwisterRandom::seed(const char* bytes) {
		uint32_t seed;
		memcpy(&seed, bytes, static_cast<size_t>(4u));
		seedMT(seed);
	}

	void MersenneTwisterRandom::seedFrom(Random& entropy) {
		char buffer[4];
		entropy.randomBytes(buffer, static_cast<MemorySize>(4u));
		seed(buffer);
	}

	void MersenneTwisterRandom::randomBytes(char* buffer, MemorySize size) {
		while(size) {
			MemorySize pull = static_cast<MemorySize>(4u);
			if(size < pull)
				pull = size;
			uint32_t x = extract();
			memcpy(buffer, &x, static_cast<size_t>(pull));
			buffer += pull;
			size -= pull;
		}
	}

	uint32_t MersenneTwisterRandom::extract() {
		if(index >= MersenneTwisterRandom::MT_N) {
			if(index > MersenneTwisterRandom::MT_N)
				seedMT(MersenneTwisterRandom::DEFAULT_SEED);
			twist();
		}
		uint32_t y = mt[index];
		y ^= (y >> MersenneTwisterRandom::MT_U) & MersenneTwisterRandom::MT_D;
		y ^= (y << MersenneTwisterRandom::MT_S) & MersenneTwisterRandom::MT_B;
		y ^= (y << MersenneTwisterRandom::MT_T) & MersenneTwisterRandom::MT_C;
		y ^= y >> 1;
		++index;
		return y;
	}

	void MersenneTwisterRandom::twist() {
		unsigned u;
		for(u = 0u; u < MersenneTwisterRandom::MT_N; ++u) {
			uint32_t x = (mt[u] & MersenneTwisterRandom::UPPER_MASK)
					+ (mt[(u + 1u) % MersenneTwisterRandom::MT_N] & MersenneTwisterRandom::LOWER_MASK);
			uint32_t xA = x >> 1;
			if(x & static_cast<uint32_t>(1u))
				xA ^= MersenneTwisterRandom::MT_A;
			mt[u] = mt[(u + MersenneTwisterRandom::MT_M) % MersenneTwisterRandom::MT_N] ^ xA;
		}
		index = 0u;
	}

}}
