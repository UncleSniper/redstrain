#include "Random.hpp"
#include "DeleteArray.hpp"

namespace redengine {
namespace util {

	Random::Random() {}

	Random::Random(const Random&) {}

	Random::~Random() {}

	void Random::seedFrom(Random& entropy) {
		MemorySize size = getSeedSize();
		if(!size) {
			seed(NULL);
			return;
		}
		DeleteArray<char> buffer(new char[size]);
		entropy.randomBytes(*buffer, size);
		seed(*buffer);
	}

}}
