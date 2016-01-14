#include "Algorithm.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	Algorithm::Algorithm() {}

	Algorithm::Algorithm(const Algorithm&) {}

	Algorithm::~Algorithm() {}

	MemorySize Algorithm::bytesNeeded(MemorySize bits) {
		return (bits + static_cast<MemorySize>(7u)) / static_cast<MemorySize>(8u);
	}

}}
