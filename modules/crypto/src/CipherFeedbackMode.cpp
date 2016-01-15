#include "CipherFeedbackMode.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	CipherFeedbackMode::CipherFeedbackMode() {}

	CipherFeedbackMode::CipherFeedbackMode(const CipherFeedbackMode& mode) : BufferingCipherMode(mode) {}

	const char* CipherFeedbackMode::preprocessBlock(CipherAlgorithm::Direction& direction, const char* iv,
			char*, MemorySize blockSize) {
		direction = CipherAlgorithm::ENCRYPT;
		if(firstBlock) {
			firstBlock = false;
			return iv;
		}
		else {
			allocateBuffer(blockSize, true);
			return bufferedBlock;
		}
	}

	const char* CipherFeedbackMode::postprocessBlock(CipherAlgorithm::Direction direction, const char*,
			char* plainText, char* cipherText, MemorySize blockSize) {
		xorBlock(cipherText, plainText, blockSize);
		if(direction == CipherAlgorithm::ENCRYPT)
			copyBlock(cipherText, blockSize);
		else
			copyBlock(plainText, blockSize);
		return cipherText;
	}

}}
