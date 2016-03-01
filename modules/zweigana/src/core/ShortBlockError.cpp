#include "ShortBlockError.hpp"

using redengine::util::FileSize;
using redengine::util::MemorySize;

namespace redengine {
namespace zweigana {
namespace core {

	ShortBlockError::ShortBlockError(FileSize blockOffset, MemorySize blockSize)
			: blockOffset(blockOffset), blockSize(blockSize) {}

	ShortBlockError::ShortBlockError(const ShortBlockError& error)
			: Error(error), DatabaseError(error), blockOffset(error.blockOffset), blockSize(error.blockSize) {}

	REDSTRAIN_DEFINE_ERROR(ShortBlockError) {
		out << "Failed to read database: Data block " << blockOffset << " is not " << blockSize << " bytes long";
	}

}}}
