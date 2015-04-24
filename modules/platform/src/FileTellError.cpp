#include "FileTellError.hpp"

namespace redengine {
namespace platform {

	FileTellError::FileTellError(File::Direction direction, File::ErrorCode code) : FileError(direction, code) {}

	FileTellError::FileTellError(const FileTellError& error) : FileError(error) {}

	REDSTRAIN_DEFINE_ERROR(FileTellError) {
		out << "Failed to retrieve read/write position in file: " << File::getErrorMessage(code);
	}

}}
