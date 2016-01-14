#include <cstring>

#include "Padding.hpp"
#include "CipherMode.hpp"
#include "BlockCipher.hpp"
#include "IllegalInputBlockSizeError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	BlockCipher::BlockCipher(CipherMode& mode, Padding& padding, MemorySize blockSize)
			: padding(padding), mode(mode), fill(static_cast<MemorySize>(0u)), blockSize(blockSize) {
		buffer = new char[static_cast<MemorySize>(2u) * blockSize];
		iv = buffer + blockSize;
		memset(iv, 0, static_cast<size_t>(blockSize));
	}

	BlockCipher::~BlockCipher() {
		delete &padding;
		delete &mode;
		delete buffer;
	}

	void BlockCipher::setInitVector(const char* ivBytes) {
		memcpy(iv, ivBytes, static_cast<size_t>(blockSize));
	}

	MemorySize BlockCipher::getBlockSize() const {
		return blockSize * static_cast<MemorySize>(8u);
	}

	MemorySize BlockCipher::transformMessage(const char* plainText, MemorySize size, char* cipherText) {
		MemorySize written = static_cast<MemorySize>(0u);
		while(size) {
			MemorySize pull = blockSize - fill;
			if(size < pull)
				pull = size;
			memcpy(buffer + fill, plainText, static_cast<size_t>(pull));
			fill += pull;
			plainText += pull;
			size -= pull;
			if(fill == blockSize) {
				fill = static_cast<MemorySize>(0u);
				processBlock(cipherText);
				written += blockSize;
			}
		}
		return written;
	}

	MemorySize BlockCipher::endMessage(char* cipherText) {
		if(getDirection() == ENCRYPT) {
			if(!fill)
				return static_cast<MemorySize>(0u);
			padding.padBlock(buffer, fill, blockSize);
			fill = static_cast<MemorySize>(0u);
			processBlock(cipherText);
			return blockSize;
		}
		else {
			if(fill)
				throw IllegalInputBlockSizeError(blockSize);
			return padding.unpadBlock(cipherText, blockSize);
		}
	}

	void BlockCipher::processBlock(char* cipherText) {
		Direction originalDirection = getDirection();
		Direction direction = originalDirection;
		const char* cipherIn = mode.preprocessBlock(direction, iv, buffer, blockSize);
		if(direction != originalDirection)
			switchDirectionForMode(direction);
		transformBlock(cipherIn, cipherText);
		if(direction != originalDirection)
			switchDirectionForMode(originalDirection);
		const char* cipherOut = mode.postprocessBlock(originalDirection, iv, buffer, cipherText, blockSize);
		if(cipherOut != cipherText)
			memcpy(cipherText, cipherOut, static_cast<size_t>(blockSize));
	}

	void BlockCipher::reset() {
		fill = static_cast<MemorySize>(0u);
	}

}}
