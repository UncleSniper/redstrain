#include "FileSeekError.hpp"

namespace redengine {
namespace platform {

	FileSeekError::FileSeekError(File::Direction direction, File::ErrorCode code) : FileError(direction, code) {}

	FileSeekError::FileSeekError(const FileSeekError& error) : Error(error), FileError(error) {}

	REDSTRAIN_DEFINE_ERROR(FileSeekError) {
		out << "Failed to seek in file: " << File::getErrorMessage(code);
	}

}}
