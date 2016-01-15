#include "CounterMode.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	CounterMode::CounterMode() : stage(NULL) {}

	CounterMode::CounterMode(const CounterMode& mode) : BufferingCipherMode(mode), stage(NULL) {}

	CounterMode::~CounterMode() {
		if(stage)
			delete[] stage;
	}

	const char* CounterMode::preprocessBlock(CipherAlgorithm::Direction& direction, const char* iv,
			char*, MemorySize blockSize) {
		direction = CipherAlgorithm::ENCRYPT;
		if(blockSize) {
			if(!stage)
				stage = new char[blockSize];
			else if(blockSize > bufferSize) {
				char* newStage = new char[blockSize];
				delete[] stage;
				stage = newStage;
			}
		}
		allocateBuffer(blockSize, true);
		MemorySize u;
		unsigned char carry = static_cast<unsigned char>(0u);
		unsigned char* ctr = reinterpret_cast<unsigned char*>(bufferedBlock) + blockSize;
		for(u = static_cast<MemorySize>(0u); u < blockSize; ++u) {
			stage[u] = iv[u] ^ bufferedBlock[u];
			unsigned char digit = *--ctr;
			carry = static_cast<unsigned char>((*ctr = static_cast<unsigned char>(digit + carry)) < digit);
		}
		return stage;
	}

	const char* CounterMode::postprocessBlock(CipherAlgorithm::Direction, const char*,
			char* plainText, char* cipherText, MemorySize blockSize) {
		xorBlock(cipherText, plainText, blockSize);
		return cipherText;
	}

}}
