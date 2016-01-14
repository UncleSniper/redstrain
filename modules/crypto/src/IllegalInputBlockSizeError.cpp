#include "IllegalInputBlockSizeError.hpp"

using redengine::util::MemorySize;

namespace redengine {
namespace crypto {

	IllegalInputBlockSizeError::IllegalInputBlockSizeError(MemorySize blockSize) : blockSize(blockSize) {}

	IllegalInputBlockSizeError::IllegalInputBlockSizeError(const IllegalInputBlockSizeError& error)
			: CryptographyError(error), blockSize(error.blockSize) {}

	REDSTRAIN_DEFINE_ERROR(IllegalInputBlockSizeError) {
		out << "Illegal input block size: Message length is not a multiple of " << blockSize << " bytes";
	}

}}
