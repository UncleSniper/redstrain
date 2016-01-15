#include <cstring>
#include "CipherAlgorithm.hpp"
#include "CipheringOutputStream.hpp"

using redengine::io::OutputStream;
using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	CipheringOutputStream::CipheringOutputStream(CipherAlgorithm& algorithm, OutputStream<char>& output)
			: ProxyOutputStream<char>(output), algorithm(algorithm),
			blockSize(Algorithm::bytesNeeded(algorithm.getBlockSize())), bufferFill(static_cast<MemorySize>(0u)),
			bufferOffset(static_cast<MemorySize>(0u)), pendingCount(static_cast<MemorySize>(0u)) {
		bufferSize = blockSize * static_cast<MemorySize>(4u);
		if(bufferSize < static_cast<MemorySize>(64u))
			bufferSize = (blockSize + static_cast<MemorySize>(63u))
					/ static_cast<MemorySize>(64u) * static_cast<MemorySize>(64u);
		buffer = new char[bufferSize];
	}

	CipheringOutputStream::CipheringOutputStream(const CipheringOutputStream& stream)
			: Stream(stream), ProxyOutputStream<char>(stream), algorithm(stream.algorithm),
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

	void CipheringOutputStream::flushBuffer() {
		if(bufferFill < bufferSize)
			return;
		// if the buffer is full, flush it to make room for more,
		// but make sure to retain one block so endMessage() can
		// pad it later if needed
		MemorySize fullEnd = bufferOffset + bufferFill - blockSize;
		if(fullEnd >= bufferSize) {
			output.write(buffer + bufferOffset, bufferSize - bufferOffset);
			fullEnd %= bufferSize;
			if(fullEnd)
				output.write(buffer, fullEnd);
		}
		else
			output.write(buffer + bufferOffset, bufferFill - blockSize);
		bufferOffset = fullEnd;
		bufferFill = blockSize;
	}

	void CipheringOutputStream::writeBlock(const char* data, MemorySize count) {
		while(count) {
			flushBuffer();
			MemorySize freeStart = (bufferOffset + bufferFill) % bufferSize;
			// how much can we draw into the buffer at offset [freeStart]?
			// this will be at least one block (the flush above ensures
			// the buffer is not full)
			MemorySize pull = bufferSize - bufferFill;
			if(freeStart + pull > bufferSize)
				pull = bufferSize - freeStart;
			// how much can we feed into the cipher to fill at most [pull] of the buffer?
			MemorySize push = pull - pendingCount;
			if(count < push)
				push = count;
			MemorySize written = algorithm.transformMessage(data, push, buffer + freeStart);
			bufferFill += written;
			pendingCount = pendingCount + push - written;
			data += push;
			count -= push;
		}
	}

	void CipheringOutputStream::endMessage() {
		if(algorithm.getDirection() == CipherAlgorithm::ENCRYPT) {
			flushBuffer();
			MemorySize freeStart = (bufferOffset + bufferFill) % bufferSize;
			MemorySize written = algorithm.endMessage(buffer + freeStart);
			bufferFill += written;
		}
		else {
			if(!bufferFill)
				return;
			MemorySize lastBlock = (bufferOffset + bufferFill - blockSize) % bufferSize;
			MemorySize unpadded = algorithm.endMessage(buffer + lastBlock);
			bufferFill -= blockSize - unpadded;
		}
		MemorySize fullEnd = bufferOffset + bufferFill;
		if(fullEnd > bufferSize) {
			output.write(buffer + bufferOffset, bufferSize - bufferOffset);
			output.write(buffer, fullEnd - bufferSize);
		}
		else
			output.write(buffer + bufferOffset, bufferFill);
		bufferFill = bufferOffset = pendingCount = static_cast<MemorySize>(0u);
	}

	void CipheringOutputStream::close() {
		endMessage();
		ProxyOutputStream<char>::close();
	}

}}
