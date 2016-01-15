#include "DigestAlgorithm.hpp"
#include "DigestOutputStream.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	DigestOutputStream::DigestOutputStream(DigestAlgorithm& algorithm) : algorithm(algorithm) {}

	DigestOutputStream::DigestOutputStream(const DigestOutputStream& stream)
			: Stream(stream), OutputStream<char>(stream), algorithm(stream.algorithm) {}

	void DigestOutputStream::writeBlock(const char* buffer, MemorySize count) {
		algorithm.digestMessage(buffer, count);
	}

}}
