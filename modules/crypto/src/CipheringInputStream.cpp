#include <cstring>
#include "CipherAlgorithm.hpp"
#include "CipheringInputStream.hpp"

using redengine::io::InputStream;
using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	CipheringInputStream::CipheringInputStream(CipherAlgorithm& algorithm, InputStream<char>& output)
			: ProxyInputStream<char>(output), algorithm(algorithm),
			blockSize(Algorithm::bytesNeeded(algorithm.getBlockSize())), bufferFill(static_cast<MemorySize>(0u)),
			bufferOffset(static_cast<MemorySize>(0u)), pendingCount(static_cast<MemorySize>(0u)) {
		bufferSize = blockSize * static_cast<MemorySize>(4u);
		if(bufferSize < static_cast<MemorySize>(64u))
			bufferSize = (blockSize + static_cast<MemorySize>(63u))
					/ static_cast<MemorySize>(64u) * static_cast<MemorySize>(64u);
		buffer = new char[bufferSize];
	}

	CipheringInputStream::CipheringInputStream(const CipheringInputStream& stream)
			: Stream(stream), ProxyInputStream<char>(stream), algorithm(stream.algorithm),
			blockSize(stream.blockSize), bufferSize(stream.bufferSize), bufferFill(stream.bufferFill),
			bufferOffset(stream.bufferOffset), pendingCount(stream.pendingCount) {
		buffer = new char[bufferSize];
		if(bufferFill) {
			MemorySize fullEnd = bufferOffset + bufferFill;
			if(fullEnd > bufferSize) {
				memcpy(buffer + bufferOffset, stream.buffer + bufferOffset,
						static_cast<size_t>(bufferSize - bufferOffset));
				memcpy(buffer, stream.buffer, static_cast<size_t>(fullEnd - bufferSize));
			}
			else
				memcpy(buffer + bufferOffset, stream.buffer + bufferOffset, static_cast<size_t>(bufferFill));
		}
	}

	CipheringInputStream::~CipheringInputStream() {
		delete[] buffer;
	}

	/*TODO
	void CipheringInputStream::fillBuffer() {
		if(atEnd)
			return;
		char readBuffer[64];
		while(bufferFill <= bufferSize - blockSize) {
			unsigned free = bufferSize - bufferFill;
			// how much can we feed into the cipher to fill the buffer?
			unsigned push = free - pendingCount;
			// how much can we read from the stream to feed at most [push]?
			unsigned pull = push > 64u ? 64u : push;
			size_t rcount = ProxyInputStream<char>::readBlock(readBuffer, static_cast<size_t>(pull));
			if(!rcount)
				break;
			unsigned available = pendingCount + static_cast<unsigned>(rcount);
			unsigned outSize = available / blockSize * blockSize;
			unsigned freeStart = (bufferOffset + bufferFill) % bufferSize;
			if(freeStart + outSize > bufferSize) {
				unsigned firstSize = bufferSize - freeStart - pendingCount;
				algorithm.transformMessage(readBuffer, static_cast<size_t>(firstSize), buffer + freeStart);
				algorithm.transformMessage(readBuffer + firstSize, rcount - static_cast<size_t>(firstSize), buffer);
			}
			else
				algorithm.transformMessage(readBuffer, rcount, buffer + freeStart);
			bufferFill += outSize;
		}
	}

	size_t CipheringInputStream::readBlock(char* data, size_t size) {
		//TODO
	}
	*/

}}
