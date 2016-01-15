#include "DigestAlgorithm.hpp"
#include "DigestingOutputStream.hpp"

using redengine::util::MemorySize;
using redengine::io::OutputStream;

namespace redengine {
namespace crypto {

	DigestingOutputStream::DigestingOutputStream(DigestAlgorithm& algorithm, OutputStream<char>& output)
			: ProxyOutputStream<char>(output), algorithm(algorithm) {}

	DigestingOutputStream::DigestingOutputStream(const DigestingOutputStream& stream)
			: Stream(stream), ProxyOutputStream<char>(stream), algorithm(stream.algorithm) {}

	void DigestingOutputStream::writeBlock(const char* buffer, MemorySize count) {
		ProxyOutputStream<char>::writeBlock(buffer, count);
		algorithm.digestMessage(buffer, count);
	}

}}
