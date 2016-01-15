#include "ElectronicCodebookMode.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	ElectronicCodebookMode::ElectronicCodebookMode() {}

	ElectronicCodebookMode::ElectronicCodebookMode(const ElectronicCodebookMode& mode) : CipherMode(mode) {}

	const char* ElectronicCodebookMode::preprocessBlock(CipherAlgorithm::Direction&, const char*,
			char* plainText, MemorySize) {
		return plainText;
	}

	const char* ElectronicCodebookMode::postprocessBlock(CipherAlgorithm::Direction, const char*,
			char*, char* cipherText, MemorySize) {
		return cipherText;
	}

}}
