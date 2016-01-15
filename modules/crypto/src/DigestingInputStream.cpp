#include "DigestAlgorithm.hpp"
#include "DigestingInputStream.hpp"

using redengine::io::InputStream;
using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	DigestingInputStream::DigestingInputStream(DigestAlgorithm& algorithm, InputStream<char>& input)
			: ProxyInputStream<char>(input), algorithm(algorithm) {}

	DigestingInputStream::DigestingInputStream(const DigestingInputStream& stream)
			: Stream(stream), ProxyInputStream<char>(stream), algorithm(stream.algorithm) {}

	MemorySize DigestingInputStream::readBlock(char* buffer, MemorySize size) {
		MemorySize count = ProxyInputStream<char>::readBlock(buffer, size);
		algorithm.digestMessage(buffer, count);
		return count;
	}

}}
