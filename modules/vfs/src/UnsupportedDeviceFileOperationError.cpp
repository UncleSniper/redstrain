#include <redstrain/util/StringUtils.hpp>

#include "UnsupportedDeviceFileOperationError.hpp"

using redengine::util::StringUtils;

namespace redengine {
namespace vfs {

	UnsupportedDeviceFileOperationError::UnsupportedDeviceFileOperationError(bool block, Stat::DeviceID device,
			Operation operation) : block(block), device(device), operation(operation) {}

	UnsupportedDeviceFileOperationError::UnsupportedDeviceFileOperationError(const
			UnsupportedDeviceFileOperationError& error) : Error(error), VFSIOError(error),
			block(error.block), device(error.device), operation(error.operation) {}

	REDSTRAIN_DEFINE_ERROR(UnsupportedDeviceFileOperationError) {
		switch(operation) {
			case TRUNCATE:
				out << "Truncation";
				break;
			case OPEN_FOR_READING:
				out << "Opening for reading";
				break;
			case OPEN_FOR_WRITING:
				out << "Opening for writing";
				break;
			case OPEN_FOR_RANDOM_ACCESS:
				out << "Opening for reading and writing";
				break;
			default:
				out << "The requested operation";
				break;
		}
		out << " is not supported for " << (block ? "block" : "character") << " device "
				<< StringUtils::toString<Stat::DeviceID>(device);
	}

}}
