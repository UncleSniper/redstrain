#include <cstring>
#include <redstrain/util/DeleteArray.hpp>

#include "DigestAlgorithm.hpp"

using redengine::util::MemorySize;
using redengine::util::DeleteArray;

namespace redengine {
namespace crypto {

	DigestAlgorithm::DigestAlgorithm() {}

	DigestAlgorithm::DigestAlgorithm(const DigestAlgorithm& algorithm) : Algorithm(algorithm) {}

	bool DigestAlgorithm::verifyDigest(const char* expectedDigest) {
		MemorySize size = getKeySize();
		DeleteArray<char> generatedDigest(new char[size]);
		getDigest(*generatedDigest);
		return !memcmp(*generatedDigest, expectedDigest, static_cast<size_t>(size));
	}

}}
