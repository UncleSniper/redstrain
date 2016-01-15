#include "CipherBlockChainingMode.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	CipherBlockChainingMode::CipherBlockChainingMode() {}

	CipherBlockChainingMode::CipherBlockChainingMode(const CipherBlockChainingMode& mode)
			: BufferingCipherMode(mode) {}

	const char* CipherBlockChainingMode::preprocessBlock(CipherAlgorithm::Direction& direction, const char* iv,
			char* plainText, MemorySize blockSize) {
		if(direction == CipherAlgorithm::ENCRYPT)
			xorWithLastBlock(iv, plainText, blockSize);
		else
			copyBlock(plainText, blockSize);
		return plainText;
	}

	const char* CipherBlockChainingMode::postprocessBlock(CipherAlgorithm::Direction direction, const char* iv,
			char*, char* cipherText, MemorySize blockSize) {
		if(direction == CipherAlgorithm::ENCRYPT)
			copyBlock(cipherText, blockSize);
		else
			xorWithLastBlock(iv, cipherText, blockSize);
		return cipherText;
	}

}}
