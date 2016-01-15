#include <cstring>

#include "BufferingCipherMode.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	BufferingCipherMode::BufferingCipherMode()
			: firstBlock(true), bufferSize(static_cast<MemorySize>(0u)), bufferedBlock(NULL) {}

	BufferingCipherMode::BufferingCipherMode(const BufferingCipherMode& mode)
			: CipherMode(mode), firstBlock(mode.firstBlock), bufferSize(mode.bufferSize) {
		if(mode.bufferedBlock) {
			bufferedBlock = new char[mode.bufferSize];
			memcpy(bufferedBlock, mode.bufferedBlock, static_cast<size_t>(bufferSize));
		}
		else
			bufferedBlock = NULL;
	}

	BufferingCipherMode::~BufferingCipherMode() {
		if(bufferedBlock)
			delete[] bufferedBlock;
	}

	void BufferingCipherMode::allocateBuffer(MemorySize blockSize, bool initialize) {
		if(!bufferedBlock) {
			if(blockSize) {
				bufferedBlock = new char[blockSize];
				bufferSize = blockSize;
				if(initialize)
					memset(bufferedBlock, 0, static_cast<size_t>(blockSize));
			}
		}
		else if(blockSize > bufferSize) {
			char* newBuffer = new char[blockSize];
			if(initialize) {
				memcpy(newBuffer, bufferedBlock, static_cast<size_t>(bufferSize));
				memset(newBuffer + bufferSize, 0, static_cast<size_t>(blockSize - bufferSize));
			}
			delete[] bufferedBlock;
			bufferedBlock = newBuffer;
			bufferSize = blockSize;
		}
	}

	void BufferingCipherMode::copyBlock(const char* data, MemorySize blockSize) {
		if(blockSize) {
			allocateBuffer(blockSize, false);
			memcpy(bufferedBlock, data, static_cast<size_t>(blockSize));
		}
	}

	void BufferingCipherMode::xorWithLastBlock(const char* initial, char* data, MemorySize blockSize) {
		allocateBuffer(blockSize, true);
		const char* source = firstBlock ? initial : bufferedBlock;
		firstBlock = false;
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < blockSize; ++u)
			data[u] ^= source[u];
	}

	void BufferingCipherMode::xorBlock(char* data, const char* key, MemorySize blockSize) {
		MemorySize u;
		for(u = static_cast<MemorySize>(0u); u < blockSize; ++u)
			data[u] ^= key[u];
	}

}}
